/*
 * Concept2 rower API.
 */

#include <stdlib.h>
#include <string.h>

#include <sys/time.h>

#include "c2api.h"
#include "c2api_priv.h"
#include "csafe.h"
#include "csafe_util.h"
#include "debug.h"
#include "utils.h"

#define HID_READ_TIMEOUT  500

/*
 * Setup a context for doing all C2 communications.
 *
 * Arguments:
 * - pointer to c2ctx_t structure.
 *
 * Returns:
 * - nothing.
 */
void c2init(c2ctx_t *ctx) {
    memset(ctx, 0, sizeof(c2ctx_t));
    ctx->last_error = C2ERR_NONE;
}

/*
 * Cleanup.
 *
 * Arguments:
 * - pointer to c2ctx_t structure.
 *
 * Returns:
 * - 0 on success.
 * - -1 on failure; call c2error() to get the last error.
 */
void c2close(c2ctx_t *ctx) {
    int i;

    if (ctx) {
        for (i = 0; i < ctx->num_rowers; i++) {
            if (ctx->rowers[i].device) {
                hid_close(ctx->rowers[i].device);
                free(ctx->rowers[i].serial_number);
            }
        }

        free(ctx->rowers);
    }
}

/*
 * Return last error.
 *
 * Arguments:
 * - none.
 *
 * Returns:
 * - value of the last error that occurred.
 *
 * Side effect:
 * - clears the last error.
 */
int c2error(c2ctx_t *ctx) {
    int e = ctx->last_error;
    ctx->last_error = C2ERR_NONE;
    return e;
}

/*
 * Discover all rowers.
 *
 * Arguments:
 * - C2 context pointer (from c2init).
 *
 * Returns:
 * - number of rowers discovered on success.
 * - -1 on failure; call c2error() to get last error.
 */
int c2discover(c2ctx_t *ctx) {
    struct hid_device_info *devices, *d;
    int num_rowers = 0;
    int i = 0;

    if (!ctx) {
        return -1;
    }

    if (hid_init() == -1) {
        ctx->last_error = C2ERR_FAILED_HID_INIT;
        return -1;
    }

    devices = hid_enumerate(HID_C2_VENDOR_ID, 0);
    for (d = devices; d; d = d->next) {
        num_rowers++;
    }

    if (num_rowers) {
        ctx->rowers = (c2rower_t*)calloc(num_rowers, sizeof(c2rower_t));
        if (!ctx->rowers) {
            ctx->last_error = C2ERR_FAILED_ALLOC_ROWERS;
            return -1;
        }

        for (d = devices, i = 0; d; d = d->next) {
            ctx->rowers[i].device = hid_open(
                    d->vendor_id,
                    d->product_id,
                    d->serial_number);

            if (ctx->rowers[i].device) {
                ctx->rowers[i].id = i;
                ctx->rowers[i].vendor_id = d->vendor_id;
                ctx->rowers[i].product_id = d->product_id;
                ctx->rowers[i].serial_number = wcsdup(d->serial_number);

                i++;
            } else {
                error("failed to open device %ls\n", d->serial_number);
            }
        }
    }

    hid_free_enumeration(devices);

    ctx->num_rowers = i;

    return i;
}

static int tx_rx(c2ctx_t *ctx, unsigned int id,
                 csafe_buffer_t *in, csafe_buffer_t *out, csafe_data_t *o) {
    int bytes;
    unsigned char tx_rx_buff[CSAFE_MAX_FRAME_SIZE+1] = { 0 };
    unsigned char *t;

    if (!ctx) {
        return -1;
    }

    tx_rx_buff[0] = 0x2;                                    /* insert HID report id */
    memcpy(tx_rx_buff+1, in->buff, in->index);

    warning("%s: request\n", __FUNCTION__);
    hexdump(in->buff, in->index);

    bytes = hid_write(ctx->rowers[id].device, tx_rx_buff, sizeof(tx_rx_buff));
    if (bytes == -1) {
        error("%s: hid_write() returned -1 (%ls)\n",
                __FUNCTION__, hid_error(ctx->rowers[id].device));
        return -1;
    }

    bytes = hid_read_timeout(ctx->rowers[id].device, tx_rx_buff, sizeof(tx_rx_buff),
            HID_READ_TIMEOUT);
    if (bytes == -1) {
        warning("%s: hid_read_timeout() returned -1 (%ls)\n",
                __FUNCTION__, hid_error(ctx->rowers[id].device));
        return -1;
    }

    memcpy(out->buff, tx_rx_buff+1, sizeof(tx_rx_buff)-1);    /* strip HID report id */
    for (t = out->buff; *t != 0xf2 && t - out->buff < CSAFE_MAX_FRAME_SIZE; t++);
    if (*t != 0xf2) {
        error("%s: failed to find end marker 0xf2\n", __FUNCTION__);
        return -1;
    }

    out->index = t - out->buff + 1;

    warning("%s: response\n", __FUNCTION__);
    hexdump(out->buff, out->index);

    return csafe_unpack(out, o);
}

static int send_to_rower(c2ctx_t *ctx, int id, csafe_buffer_t *out,
                         csafe_buffer_t *in, csafe_data_t *d) {
    int i = 0;
    int num_errors = 0;

    /*
     * XXX data returned is for the last rower
     */
    if (id == -1) {
        for (i = 0; i < ctx->num_rowers; i++) {
            num_errors += tx_rx(ctx, i, out, in, d) == -1;
        }
    } else {
        num_errors += tx_rx(ctx, id, out, in, d) == -1;
    }

    return num_errors;
}

/*
 */
int c2syncClock(c2ctx_t *ctx, int id) {
    csafe_buffer_t b;
    csafe_data_t d = { 0 };
    struct timeval tv = { 0, 0 };
    struct tm r;

    csafe_init_data(&d);

    if (gettimeofday(&tv, NULL) == -1) {
        ctx->last_error = C2ERR_FAILED_GETTIMEOFDAY;
        return -1;
    }

    if (!localtime_r(&tv.tv_sec, &r)) {
        ctx->last_error = C2ERR_FAILED_LOCALTIME;
        return -1;
    }

    d.setdate_year = r.tm_year;
    d.setdate_month = r.tm_mon + 1;
    d.setdate_day = r.tm_mday;

    d.settime_hour = r.tm_hour;
    d.settime_minute = r.tm_min;
    d.settime_second = r.tm_sec;

    csafe_init_standard_cmd(&b);
    csafe_push_cmd(CSAFE_SETDATE_CMD, &b, &d);
    csafe_push_cmd(CSAFE_SETTIME_CMD, &b, &d);
    csafe_end_cmd(&b);

    return send_to_rower(ctx, id, &b, &b, &d);
}

int c2reset(c2ctx_t *c, int id) {
    csafe_buffer_t b;
    csafe_data_t d;

    csafe_init_data(&d);
    csafe_init_standard_cmd(&b);
    csafe_push_cmd(CSAFE_RESET_CMD, &b, &d);
    csafe_end_cmd(&b);

    return send_to_rower(c, id, &b, &b, &d);
}

int c2setWorkoutTime(c2ctx_t *c, int id, int h, int m, int s) {
    csafe_buffer_t b;
    csafe_data_t d;

    csafe_init_data(&d);

    d.settwork_hours = h;
    d.settwork_minutes = m;
    d.settwork_seconds = s;

    csafe_init_standard_cmd(&b);
    csafe_push_cmd(CSAFE_SETTWORK_CMD, &b, &d);
    csafe_end_cmd(&b);

    return send_to_rower(c, id, &b, &b, &d);
}

int c2setWorkoutDistance(c2ctx_t *c, int id, int distance) {
    csafe_buffer_t b;
    csafe_data_t d;

    csafe_init_data(&d);

    d.sethorizontal = distance;
    d.sethorizontal_units = 0;          /* XXX spec and header ambiguous? */

    csafe_init_standard_cmd(&b);
    csafe_push_cmd(CSAFE_SETHORIZONTAL_CMD, &b, &d);
    csafe_end_cmd(&b);

    return send_to_rower(c, id, &b, &b, &d);
}

int c2setWorkoutCalories(c2ctx_t *c, int id, int calories) {
    csafe_buffer_t b;
    csafe_data_t d;

    csafe_init_data(&d);

    d.setcalories = calories;

    csafe_init_standard_cmd(&b);
    csafe_push_cmd(CSAFE_SETCALORIES_CMD, &b, &d);
    csafe_end_cmd(&b);

    return send_to_rower(c, id, &b, &b, &d);
}

int c2setProgram(c2ctx_t *c, int id, int program) {
    csafe_buffer_t b;
    csafe_data_t d;

    csafe_init_data(&d);

    d.program = program;

    csafe_init_standard_cmd(&b);
    csafe_push_cmd(CSAFE_SETPROGRAM_CMD, &b, &d);
    csafe_end_cmd(&b);

    return send_to_rower(c, id, &b, &b, &d);
}

/*
 * http://www.concept2.com.au/service/monitors/pm5/how-to-use/using-monitor-display-options
 *
 * time / distance elapsed / remaining
 * pace per 500m / calories / watts
 * average pace per 500m / calories / watts
 * split time, metres, pace, calories or watts
 * heart rate
 * projected finish:
 *   time workokuts: number of metres at end of workout
 *   distance workout: time to complete workout (just row: time is 30 minutes)
 *   interval workout: projected finish or time for interval
*/
int c2get(c2ctx_t *c, int id, char *cmd, csafe_data_t *d) {
    csafe_buffer_t b;

    csafe_init_data(d);
    csafe_init_standard_cmd(&b);

    if (!strcmp(cmd, "status")) {
        csafe_push_cmd(CSAFE_GETSTATUS_CMD, &b, d);
        csafe_push_cmd(CSAFE_GETERRORCODE_CMD, &b, d);
        csafe_push_cmd(CSAFE_PM_GET_ERRORVALUE, &b, d);
    } else if (!strcmp(cmd, "monitor")) {
        csafe_push_cmd(CSAFE_GETTWORK_CMD, &b, d);
        csafe_push_cmd(CSAFE_GETHORIZONTAL_CMD, &b, d);
        csafe_push_cmd(CSAFE_GETPACE_CMD, &b, d);
        csafe_push_cmd(CSAFE_GETPOWER_CMD, &b, d);
        csafe_push_cmd(CSAFE_GETCALORIES_CMD, &b, d);
        csafe_push_cmd(CSAFE_GETCADENCE_CMD, &b, d);
        csafe_push_cmd(CSAFE_GETHRCUR_CMD, &b, d);
    } else if (!strcmp(cmd, "forceplot")) {
        csafe_push_cmd(CSAFE_PM_GET_FORCEPLOTDATA, &b, d);
        csafe_push_cmd(CSAFE_PM_GET_HEARTBEATDATA, &b, d);
    } else if (!strcmp(cmd, "workout")) {
        csafe_push_cmd(CSAFE_PM_GET_WORKTIME, &b, d);
        csafe_push_cmd(CSAFE_PM_GET_WORKDISTANCE, &b, d);
        csafe_push_cmd(CSAFE_PM_GET_STROKESTATE, &b, d);
        csafe_push_cmd(CSAFE_PM_GET_WORKOUTTYPE, &b, d);
        csafe_push_cmd(CSAFE_PM_GET_DRAGFACTOR, &b, d);
        csafe_push_cmd(CSAFE_PM_GET_RESTTIME, &b, d);
        csafe_push_cmd(CSAFE_PM_GET_INTERVALTYPE, &b, d);
        csafe_push_cmd(CSAFE_PM_GET_WORKOUTINTERVALCOUNT, &b, d);
    } else if (!strcmp(cmd, "erg")) {
        csafe_push_cmd(CSAFE_GETODOMETER_CMD, &b, d);
        csafe_push_cmd(CSAFE_GETVERSION_CMD, &b, d);
        csafe_push_cmd(CSAFE_GETSERIAL_CMD, &b, d);
    } else {
        c->last_error = C2ERR_UNIMPLEMENTED;
        return -1;
    }

    csafe_end_cmd(&b);

    return send_to_rower(c, id, &b, &b, d);
}

int c2setState(c2ctx_t *c, int id, char *cmd) {
    csafe_cmd command;
    csafe_buffer_t b;
    csafe_data_t d;
    bool ok = false;

    /*
     * Get status first.
     */
    csafe_init_data(&d);
    csafe_init_standard_cmd(&b);
    csafe_push_cmd(CSAFE_GETSTATUS_CMD, &b, &d);
    csafe_end_cmd(&b);
    send_to_rower(c, id, &b, &b, &d);

    /*
     * Figure out requested command.
     */
    command =
        !strcmp(cmd, "finished") ? CSAFE_GOFINISHED_CMD :
        !strcmp(cmd, "idle") ? CSAFE_GOIDLE_CMD :
        !strcmp(cmd, "inuse") ? CSAFE_GOINUSE_CMD :
        !strcmp(cmd, "haveid") ? CSAFE_GOHAVEID_CMD :
        !strcmp(cmd, "ready") ? CSAFE_GOREADY_CMD :
        CSAFE_GETSTATUS_CMD;

    /*
     * Do we need to transition?
     */
    if ((d.status == CSAFE_MACHINE_STATE_READY && command == CSAFE_GOREADY_CMD) ||
        (d.status == CSAFE_MACHINE_STATE_IDLE && command == CSAFE_GOIDLE_CMD) ||
        (d.status == CSAFE_MACHINE_STATE_HAVEID && command == CSAFE_GOHAVEID_CMD) ||
        (d.status == CSAFE_MACHINE_STATE_INUSE && command == CSAFE_GOINUSE_CMD) ||
        (d.status == CSAFE_MACHINE_STATE_FINISHED && command == CSAFE_GOFINISHED_CMD)) {
        return 0;
    }

    /*
     * Allowed to transition?
     */
    switch (d.status) {
        case CSAFE_MACHINE_STATE_READY:
            ok = command == CSAFE_GOIDLE_CMD || command == CSAFE_GOINUSE_CMD;
            break;

        case CSAFE_MACHINE_STATE_IDLE:
            ok = command == CSAFE_GOHAVEID_CMD || command == CSAFE_GOINUSE_CMD;
            break;

        case CSAFE_MACHINE_STATE_HAVEID:
            ok = command == CSAFE_GOIDLE_CMD || command == CSAFE_GOINUSE_CMD;
            break;

        case CSAFE_MACHINE_STATE_PAUSED:
            ok = command == CSAFE_GOFINISHED_CMD;
            break;

        case CSAFE_MACHINE_STATE_FINISHED:
            ok = command == CSAFE_GOREADY_CMD || command == CSAFE_GOIDLE_CMD;
            break;

        case CSAFE_MACHINE_STATE_OFFLINE:
            ok = command == CSAFE_GOREADY_CMD;
            break;

        default:
            break;
    }

    /*
     * Do the transition.
     */
    if (command != CSAFE_GETSTATUS_CMD && ok) {
        csafe_init_data(&d);
        csafe_init_standard_cmd(&b);
        csafe_push_cmd(command, &b, &d);
        csafe_end_cmd(&b);
        return send_to_rower(c, id, &b, &b, &d);
    }

    return -1;
}
