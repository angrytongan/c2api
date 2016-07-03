#include <string.h>

#include "csafe.h"
#include "debug.h"
#include "utils.h"

#ifdef DEBUG
#include "csafe_util.h"
#endif

#define FRAME_START_EXTENDED    0xf0
#define FRAME_START_STANDARD    0xf1
#define FRAME_STOP              0xf2
#define FRAME_STUFF             0xf3

/* short commands */
#define CSAFE_GETSTATUS_CMD_HEX         0x80
#define CSAFE_RESET_CMD_HEX             0x81
#define CSAFE_GOIDLE_CMD_HEX            0x82
#define CSAFE_GOHAVEID_CMD_HEX          0x83
#define CSAFE_GOINUSE_CMD_HEX           0x85
#define CSAFE_GOFINISHED_CMD_HEX        0x86
#define CSAFE_GOREADY_CMD_HEX           0x87
#define CSAFE_BADID_CMD_HEX             0x88
#define CSAFE_GETVERSION_CMD_HEX        0x91
#define CSAFE_GETID_CMD_HEX             0x92
#define CSAFE_GETUNITS_CMD_HEX          0x93
#define CSAFE_GETSERIAL_CMD_HEX         0x94
#define CSAFE_GETLIST_CMD_HEX           0x98
#define CSAFE_GETUTILIZATION_CMD_HEX    0x99
#define CSAFE_GETMOTORCURRENT_CMD_HEX   0x9a
#define CSAFE_GETODOMETER_CMD_HEX       0x9b
#define CSAFE_GETERRORCODE_CMD_HEX      0x9c
#define CSAFE_GETSERVICECODE_CMD_HEX    0x9d
#define CSAFE_GETUSERCFG1_CMD_HEX       0x9e
#define CSAFE_GETUSERCFG2_CMD_HEX       0x9f
#define CSAFE_GETTWORK_CMD_HEX          0xa0
#define CSAFE_GETHORIZONTAL_CMD_HEX     0xa1
#define CSAFE_GETVERTICAL_CMD_HEX       0xa2
#define CSAFE_GETCALORIES_CMD_HEX       0xa3
#define CSAFE_GETPROGRAM_CMD_HEX        0xa4
#define CSAFE_GETSPEED_CMD_HEX          0xa5
#define CSAFE_GETPACE_CMD_HEX           0xa6
#define CSAFE_GETCADENCE_CMD_HEX        0xa7
#define CSAFE_GETGRADE_CMD_HEX          0xa8
#define CSAFE_GETGEAR_CMD_HEX           0xa9
#define CSAFE_GETUPLIST_CMD_HEX         0xaa
#define CSAFE_GETUSERINFO_CMD_HEX       0xab
#define CSAFE_GETTORQUE_CMD_HEX         0xac
#define CSAFE_GETHRCUR_CMD_HEX          0xb0
#define CSAFE_GETHRTZONE_CMD_HEX        0xb2
#define CSAFE_GETMETS_CMD_HEX           0xb3
#define CSAFE_GETPOWER_CMD_HEX          0xb4
#define CSAFE_GETHRAVG_CMD_HEX          0xb5
#define CSAFE_GETHRMAX_CMD_HEX          0xb6
#define CSAFE_GETUSERDATA1_CMD_HEX      0xbe
#define CSAFE_GETUSERDATA2_CMD_HEX      0xbf
#define CSAFE_GETAUDIOCHANNEL_CMD_HEX   0xc0
#define CSAFE_GETAUDIOVOLUME_CMD_HEX    0xc1
#define CSAFE_GETAUDIOMUTE_CMD_HEX      0xc2
#define CSAFE_ENDTEXT_CMD_HEX           0xe0
#define CSAFE_DISPLAYPOPUP_CMD_HEX      0xe1
#define CSAFE_GETPOPUPSTATUS_CMD_HEX    0xe5

/* long commands */
#define CSAFE_AUTOUPLOAD_CMD_HEX        0x01
#define CSAFE_UPLIST_CMD_HEX            0x02
#define CSAFE_UPSTATUSSEC_CMD_HEX       0x04
#define CSAFE_UPLISTSEC_CMD_HEX         0x05
#define CSAFE_IDDIGITS_CMD_HEX          0x10
#define CSAFE_SETTIME_CMD_HEX           0x11
#define CSAFE_SETDATE_CMD_HEX           0x12
#define CSAFE_SETTIMEOUT_CMD_HEX        0x13
#define CSAFE_SETUSERCFG1_CMD_HEX       0x1a
#define CSAFE_SETUSERCFG2_CMD_HEX       0x1b
#define CSAFE_SETTWORK_CMD_HEX          0x20
#define CSAFE_SETHORIZONTAL_CMD_HEX     0x21
#define CSAFE_SETVERTICAL_CMD_HEX       0x22
#define CSAFE_SETCALORIES_CMD_HEX       0x23
#define CSAFE_SETPROGRAM_CMD_HEX        0x24
#define CSAFE_SETSPEED_CMD_HEX          0x25
#define CSAFE_SETGRADE_CMD_HEX          0x28
#define CSAFE_SETGEAR_CMD_HEX           0x29
#define CSAFE_SETUSERINFO_CMD_HEX       0x2b
#define CSAFE_SETTORQUE_CMD_HEX         0x2c
#define CSAFE_SETLEVEL_CMD_HEX          0x2d
#define CSAFE_SETTARGETHR_CMD_HEX       0x30
#define CSAFE_SETMETS_CMD_HEX           0x33
#define CSAFE_SETPOWER_CMD_HEX          0x34
#define CSAFE_SETHRZONE_CMD_HEX         0x35
#define CSAFE_SETHRMAX_CMD_HEX          0x36
#define CSAFE_SETCHANNELRANGE_CMD_HEX   0x40
#define CSAFE_SETVOLUMERANGE_CMD_HEX    0x41
#define CSAFE_SETAUDIOMUTE_CMD_HEX      0x42
#define CSAFE_SETAUDIOCHANNEL_CMD_HEX   0x43
#define CSAFE_SETAUDIOVOLUME_CMD_HEX    0x44
#define CSAFE_STARTTEXT_CMD_HEX         0x60
#define CSAFE_APPENDTEXT_CMD_HEX        0x61
#define CSAFE_GETTEXTSTATUS_CMD_HEX     0x65
#define CSAFE_GETCAPS_CMD_HEX           0x70
#define CSAFE_SETPMCFG_CMD_HEX          0x76
#define CSAFE_SETPMDATA_CMD_HEX         0x77
#define CSAFE_GETPMCFG_CMD_HEX          0x7e
#define CSAFE_GETPMDATA_CMD_HEX         0x7f

/* pm specific short commands */
#define CSAFE_PM_GET_WORKOUTTYPE_HEX    0x89
#define CSAFE_PM_GET_DRAGFACTOR_HEX     0xc1
#define CSAFE_PM_GET_STROKESTATE_HEX    0xbf
#define CSAFE_PM_GET_WORKTIME_HEX       0xa0
#define CSAFE_PM_GET_WORKDISTANCE_HEX   0xa3
#define CSAFE_PM_GET_ERRORVALUE_HEX     0xc9
#define CSAFE_PM_GET_WORKOUTSTATE_HEX   0x8d
#define CSAFE_PM_GET_WORKOUTINTERVALCOUNT_HEX   0x9f
#define CSAFE_PM_GET_INTERVALTYPE_HEX   0x8e
#define CSAFE_PM_GET_RESTTIME_HEX       0xcf

/* pm specific long commands */
#define CSAFE_PM_SET_SPLITDURATION_HEX  0x05
#define CSAFE_PM_GET_FORCEPLOTDATA_HEX  0x6b
#define CSAFE_PM_SET_SCREENERRORMODE_HEX    0x27
#define CSAFE_PM_GET_HEARTBEATDATA_HEX  0x6c

/* buffer manipulation */
#define BUFF_TX_INIT(C) warning("%s", __FUNCTION__); b->index = 0; b->buff[b->index++] = C
#define BUFF_TX_APPEND_BYTE(C) b->buff[b->index++] = (C & 0xff)
#define BUFF_TX_APPEND_BYTES(T,N) memcpy(b->buff+b->index, T, N); b->index += N
#define BUFF_TX_APPEND_VAL(S, N) val2bytes(S, N, b->buff + b->index); b->index += N
#define BUFF_TX_END() return b->index

#define BUFF_RX_INIT(C, N) warning("%s", __FUNCTION__); { int i = 2; if (b->buff[0] != C || b->buff[1] != N) { error("init failed"); return -1; }
#define BUFF_RX_POP_BYTE(O) O = b->buff[i++]
#define BUFF_RX_POP_BYTES(T,N) memcpy(T, b->buff+i, N); i += N
#define BUFF_RX_POP_VAL(O, N) bytes2val(b->buff+i, N, &O); i += N
#define BUFF_RX_END() return i; }
#define BUFF_RX_PM3_CHK(C, N) if (b->buff[i] != C || b->buff[i+1] != N) { error("check failed"); return -1; } i+=2;
#define BUFF_RX_INC(N) i += N;

typedef int (*csafe_handler_t)(csafe_buffer_t*, csafe_data_t*);

static void byte_stuff(csafe_buffer_t *b) {
    csafe_buffer_t t;
    unsigned int i;
    unsigned char *c;
    unsigned char *e;

    for (i = 0, c = b->buff, e = b->buff + b->index; c != e; c++) {
        if (*c == FRAME_START_EXTENDED ||
                *c == FRAME_START_STANDARD ||
                *c == FRAME_STOP ||
                *c == FRAME_STUFF) {
            t.buff[i++] = FRAME_STUFF;
            t.buff[i++] = *c - FRAME_START_EXTENDED;
        } else {
            t.buff[i++] = *c;
        }
    }

    memcpy(b->buff, t.buff, i);     /* save doing memmove() repeatedly */
    b->index = i;
}

static void byte_unstuff(csafe_buffer_t *b) {
    csafe_buffer_t t;
    unsigned int i;
    unsigned char *c;
    unsigned char *e;

    for (i = 0, c = b->buff, e = b->buff + b->index; c != e; i++) {
        if (*c == FRAME_STUFF) {
            c++;
            t.buff[i] = *c + FRAME_START_EXTENDED;
        } else {
            t.buff[i] = *c;
        }
        c++;
    }

    memcpy(b->buff, t.buff, i);
    b->index = i;
}

static void strip_start_cksum_stop(csafe_buffer_t *b) {
    b->index -= 3;
    memmove(b->buff, b->buff+1, b->index);
}

static int csafe_getstatus_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETSTATUS_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_getstatus_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_GETSTATUS_CMD_HEX, 1);
    BUFF_RX_POP_BYTE(o->status);
    BUFF_RX_END();
}
/*
 * Set: CSAFE machine reset
 */
static int csafe_reset_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_RESET_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_reset_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return 0;
}
static int csafe_goidle_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GOIDLE_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_goidle_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return 0;
}
static int csafe_gohaveid_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GOHAVEID_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_gohaveid_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return 0;
}
static int csafe_goinuse_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GOINUSE_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_goinuse_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return 0;
}
static int csafe_gofinished_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GOFINISHED_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_gofinished_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return 0;
}
static int csafe_goready_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GOREADY_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_goready_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return 0;
}
static int csafe_badid_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_BADID_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_badid_resp(csafe_buffer_t *b, csafe_data_t *o) {
    o->status = b->buff[0];
    return 0;
}
static int csafe_getversion_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETVERSION_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_getversion_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_GETVERSION_CMD_HEX, 7);
    BUFF_RX_POP_BYTE(o->mfg_id);
    BUFF_RX_POP_BYTE(o->cid);
    BUFF_RX_POP_BYTE(o->model);
    BUFF_RX_POP_VAL(o->hw_version, 2);
    BUFF_RX_POP_VAL(o->sw_version, 2);
    BUFF_RX_END();
}
static int csafe_getid_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETID_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_getid_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;
}
static int csafe_getunits_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETUNITS_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_getunits_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_GETUNITS_CMD_HEX, 1);
    BUFF_RX_POP_BYTE(o->units);
    BUFF_RX_END();
}
static int csafe_getserial_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETSERIAL_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_getserial_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_GETSERIAL_CMD_HEX, 9);
    BUFF_RX_POP_BYTES(o->serial, 9);
    BUFF_RX_END();
}
static int csafe_getlist_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getlist_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getutilization_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getutilization_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getmotorcurrent_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getmotorcurrent_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getodometer_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETODOMETER_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_getodometer_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_GETODOMETER_CMD_HEX, 5);
    BUFF_RX_POP_VAL(o->odometer, 4);
    BUFF_RX_POP_VAL(o->odometer_units, 1);
    BUFF_RX_END();
}
static int csafe_geterrorcode_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETERRORCODE_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_geterrorcode_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_GETERRORCODE_CMD_HEX, 3);
    BUFF_RX_POP_VAL(o->error, 3);
    BUFF_RX_END();
}
static int csafe_getservicecode_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getservicecode_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getusercfg2_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getusercfg2_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_gettwork_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETTWORK_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_gettwork_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_GETTWORK_CMD_HEX, 3);
    BUFF_RX_POP_BYTE(o->twork_hours);
    BUFF_RX_POP_BYTE(o->twork_minutes);
    BUFF_RX_POP_BYTE(o->twork_seconds);
    BUFF_RX_END();
}
static int csafe_gethorizontal_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETHORIZONTAL_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_gethorizontal_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_GETHORIZONTAL_CMD_HEX, 3);
    BUFF_RX_POP_VAL(o->horizontal, 2);
    BUFF_RX_POP_BYTE(o->horizontal_units);
    BUFF_RX_END();
}
static int csafe_getvertical_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getvertical_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getcalories_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETCALORIES_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_getcalories_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_GETCALORIES_CMD_HEX, 2);
    BUFF_RX_POP_VAL(o->calories, 2);
    BUFF_RX_END();
}
static int csafe_getprogram_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETPROGRAM_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_getprogram_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_GETPROGRAM_CMD_HEX, 1);
    BUFF_RX_POP_BYTE(o->program);
    BUFF_RX_END();
}
static int csafe_getspeed_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;
}
static int csafe_getspeed_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;
}
static int csafe_getpace_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETPACE_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_getpace_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_GETPACE_CMD_HEX, 3);
    BUFF_RX_POP_VAL(o->pace, 2);
    BUFF_RX_POP_BYTE(o->pace_units);
    BUFF_RX_END();
}
static int csafe_getcadence_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETCADENCE_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_getcadence_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_GETCADENCE_CMD_HEX, 3);
    BUFF_RX_POP_VAL(o->cadence, 2);
    BUFF_RX_POP_BYTE(o->cadence_units);
    BUFF_RX_END();
}
static int csafe_getgrade_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getgrade_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getgear_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getgear_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getuplist_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getuplist_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getuserinfo_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETUSERINFO_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_getuserinfo_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_GETUSERINFO_CMD_HEX, 5);
    BUFF_RX_POP_VAL(o->weight, 2);
    BUFF_RX_POP_BYTE(o->weight_units);
    BUFF_RX_POP_BYTE(o->age);
    BUFF_RX_POP_BYTE(o->gender);
    BUFF_RX_END();
}
static int csafe_gettorque_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_gettorque_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_gethrcur_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETHRCUR_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_gethrcur_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_GETHRCUR_CMD_HEX, 1);
    BUFF_RX_POP_BYTE(o->hr);
    BUFF_RX_END();
}
static int csafe_gethrtzone_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_gethrtzone_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getmets_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getmets_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getpower_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETPOWER_CMD_HEX);
    BUFF_TX_END();
}
static int csafe_getpower_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_GETPOWER_CMD_HEX, 3);
    BUFF_RX_POP_VAL(o->power, 2);
    BUFF_RX_POP_BYTE(o->power_units);
    BUFF_RX_END();
}
static int csafe_gethravg_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_gethravg_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_gethrmax_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_gethrmax_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getuserdata1_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getuserdata1_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getuserdata2_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getuserdata2_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getaudiochannel_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getaudiochannel_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getaudiovolume_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getaudiovolume_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getaudiomute_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getaudiomute_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_endtext_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;
}
static int csafe_endtext_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_displaypopup_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_displaypopup_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getpopupstatus_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getpopupstatus_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_autoupload_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* no effect */
}
static int csafe_autoupload_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* no effect */
}
static int csafe_uplist_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_uplist_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_upstatussec_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_upstatussec_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_uplistsec_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_uplistsec_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_iddigits_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_iddigits_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_settime_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETTIME_CMD_HEX);
    BUFF_TX_APPEND_BYTE(3);
    BUFF_TX_APPEND_BYTE(o->settime_hour);
    BUFF_TX_APPEND_BYTE(o->settime_minute);
    BUFF_TX_APPEND_BYTE(o->settime_second);
    BUFF_TX_END();
}
static int csafe_settime_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return 0;
}
static int csafe_setdate_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETDATE_CMD_HEX);
    BUFF_TX_APPEND_BYTE(3);
    BUFF_TX_APPEND_BYTE(o->setdate_year);
    BUFF_TX_APPEND_BYTE(o->setdate_month);
    BUFF_TX_APPEND_BYTE(o->setdate_day);
    BUFF_TX_END();
}
static int csafe_setdate_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return 0;
}
static int csafe_settimeout_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETTIMEOUT_CMD_HEX);
    BUFF_TX_APPEND_BYTE(1);
    BUFF_TX_APPEND_BYTE(o->settimeout);
    BUFF_TX_END();
}
static int csafe_settimeout_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return 0;
}
static int csafe_settwork_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETTWORK_CMD_HEX);
    BUFF_TX_APPEND_BYTE(3);
    BUFF_TX_APPEND_BYTE(o->settwork_hours);
    BUFF_TX_APPEND_BYTE(o->settwork_minutes);
    BUFF_TX_APPEND_BYTE(o->settwork_seconds);
    BUFF_TX_END();
}
static int csafe_settwork_resp(csafe_buffer_t *b, csafe_data_t *o) {
    o->status = b->buff[0];
    return -1;
}
static int csafe_sethorizontal_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETHORIZONTAL_CMD_HEX);
    BUFF_TX_APPEND_BYTE(3);
    BUFF_TX_APPEND_VAL(o->sethorizontal, 2);
    BUFF_TX_APPEND_BYTE(o->sethorizontal_units);
    BUFF_TX_END();
}
static int csafe_sethorizontal_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return 0;
}
static int csafe_setvertical_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setvertical_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setcalories_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETCALORIES_CMD_HEX);
    BUFF_TX_APPEND_BYTE(2);
    BUFF_TX_APPEND_VAL(o->setcalories, 2);
    BUFF_TX_END();
}
static int csafe_setcalories_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return 0;
}
static int csafe_setprogram_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETPROGRAM_CMD_HEX);
    BUFF_TX_APPEND_BYTE(2);
    BUFF_TX_APPEND_VAL(o->setprogram, 2);
    BUFF_TX_END();
}
static int csafe_setprogram_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return 0;
}
static int csafe_setspeed_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setspeed_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setgrade_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setgrade_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setgear_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setgear_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setuserinfo_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setuserinfo_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_settorque_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_settorque_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setlevel_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setlevel_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_settargethr_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_settargethr_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setmets_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setmets_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setpower_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETPOWER_CMD_HEX);
    BUFF_TX_APPEND_BYTE(3);
    BUFF_TX_APPEND_VAL(o->setpower, 2);
    BUFF_TX_APPEND_BYTE(o->setpower_units);
    BUFF_TX_END();
}
static int csafe_setpower_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return 0;
}
static int csafe_sethrzone_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_sethrzone_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_sethrmax_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_sethrmax_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setchannelrange_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setchannelrange_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setvolumerange_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setvolumerange_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setaudiomute_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setaudiomute_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setaudiochannel_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setaudiochannel_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setaudiovolume_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setaudiovolume_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_starttext_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_starttext_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_appendtext_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_appendtext_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_gettextstatus_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_gettextstatus_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getcaps_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_GETCAPS_CMD_HEX);
    BUFF_TX_APPEND_BYTE(1);
    BUFF_TX_APPEND_BYTE(o->capability_code);
    BUFF_TX_END();
}
static int csafe_getcaps_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* XXX finish this */
}
static int csafe_setpmcfg_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* not available for public use */
}
static int csafe_setpmcfg_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* not available for public use */
}
static int csafe_setpmdata_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* not available for public use */
}
static int csafe_setpmdata_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* not available for public use */
}
static int csafe_getpmcfg_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* not available for public use */
}
static int csafe_getpmcfg_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* not available for public use */
}
static int csafe_getpmdata_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* not available for public use */
}
static int csafe_getpmdata_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* not available for public use */
}
static int csafe_pm_get_workouttype_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETUSERCFG1_CMD_HEX);
    BUFF_TX_APPEND_BYTE(1);
    BUFF_TX_APPEND_BYTE(CSAFE_PM_GET_WORKOUTTYPE);
    BUFF_TX_END();
}
static int csafe_pm_get_workouttype_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_SETUSERCFG1_CMD_HEX, 3);
    BUFF_RX_PM3_CHK(CSAFE_PM_GET_WORKOUTTYPE_HEX, 1);
    BUFF_RX_POP_BYTE(o->workout_type);
    BUFF_RX_END();
}
static int csafe_pm_get_dragfactor_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETUSERCFG1_CMD_HEX);
    BUFF_TX_APPEND_BYTE(1);
    BUFF_TX_APPEND_BYTE(CSAFE_PM_GET_DRAGFACTOR_HEX);
    BUFF_TX_END();
}
static int csafe_pm_get_dragfactor_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_SETUSERCFG1_CMD_HEX, 3);
    BUFF_RX_PM3_CHK(CSAFE_PM_GET_DRAGFACTOR_HEX, 1);
    BUFF_RX_POP_BYTE(o->drag_factor);
    BUFF_RX_END();
}
static int csafe_pm_get_strokestate_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETUSERCFG1_CMD_HEX);
    BUFF_TX_APPEND_BYTE(1);
    BUFF_TX_APPEND_BYTE(CSAFE_PM_GET_STROKESTATE_HEX);
    BUFF_TX_END();
}
static int csafe_pm_get_strokestate_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_SETUSERCFG1_CMD_HEX, 3);
    BUFF_RX_PM3_CHK(CSAFE_PM_GET_STROKESTATE_HEX, 1);
    BUFF_RX_POP_BYTE(o->stroke_state);
    BUFF_RX_END();
}
static int csafe_pm_get_worktime_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETUSERCFG1_CMD_HEX);
    BUFF_TX_APPEND_BYTE(1);
    BUFF_TX_APPEND_BYTE(CSAFE_PM_GET_WORKTIME_HEX);
    BUFF_TX_END();
}
static int csafe_pm_get_worktime_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_SETUSERCFG1_CMD_HEX, 6);
    BUFF_RX_PM3_CHK(CSAFE_PM_GET_WORKTIME_HEX, 5);
    BUFF_RX_POP_VAL(o->work_time, 4);
    BUFF_RX_POP_BYTE(o->work_time_frac);
    BUFF_RX_END();
}
static int csafe_pm_get_workdistance_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETUSERCFG1_CMD_HEX);
    BUFF_TX_APPEND_BYTE(1);
    BUFF_TX_APPEND_BYTE(CSAFE_PM_GET_WORKTIME_HEX);
    BUFF_TX_END();
}
static int csafe_pm_get_workdistance_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_SETUSERCFG1_CMD_HEX, 6);
    BUFF_RX_PM3_CHK(CSAFE_PM_GET_WORKDISTANCE_HEX, 5);
    BUFF_RX_POP_VAL(o->work_distance, 4);
    BUFF_RX_POP_BYTE(o->work_distance_frac);
    BUFF_RX_END();
}
static int csafe_pm_get_errorvalue_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETUSERCFG1_CMD_HEX);
    BUFF_TX_APPEND_BYTE(1);
    BUFF_TX_APPEND_BYTE(CSAFE_PM_GET_ERRORVALUE_HEX);
    BUFF_TX_END();
}
static int csafe_pm_get_errorvalue_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_SETUSERCFG1_CMD_HEX, 3);
    BUFF_RX_PM3_CHK(CSAFE_PM_GET_ERRORVALUE_HEX, 2);
    BUFF_RX_POP_VAL(o->error_value, 2);
    BUFF_RX_END();
}
static int csafe_pm_get_workoutstate_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETUSERCFG1_CMD_HEX);
    BUFF_TX_APPEND_BYTE(1);
    BUFF_TX_APPEND_BYTE(CSAFE_PM_GET_WORKOUTSTATE_HEX);
    BUFF_TX_END();
}
static int csafe_pm_get_workoutstate_resp(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_RX_INIT(CSAFE_SETUSERCFG1_CMD, 3);
    BUFF_RX_PM3_CHK(CSAFE_PM_GET_WORKOUTSTATE_HEX, 1);
    BUFF_RX_POP_BYTE(o->workout_state);
    BUFF_RX_END();
}
static int csafe_pm_get_workoutintervalcount_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;
}
static int csafe_pm_get_workoutintervalcount_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;
}
static int csafe_pm_get_intervaltype_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;
}
static int csafe_pm_get_intervaltype_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;
}
static int csafe_pm_get_resttime_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;
}
static int csafe_pm_get_resttime_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;
}
static int csafe_pm_set_splitduration_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETUSERCFG1_CMD_HEX);
    BUFF_TX_APPEND_BYTE(7);
    BUFF_TX_APPEND_BYTE(CSAFE_PM_SET_SPLITDURATION_HEX);
    BUFF_TX_APPEND_BYTE(5);
    BUFF_TX_APPEND_BYTE(o->setsplit_duration_type);
    BUFF_TX_APPEND_VAL(o->setsplit_duration, 4);
    BUFF_TX_END();
}
static int csafe_pm_set_splitduration_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return 0;
}
static int csafe_pm_get_forceplotdata_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETUSERCFG1_CMD_HEX);
    BUFF_TX_APPEND_BYTE(3);
    BUFF_TX_APPEND_BYTE(CSAFE_PM_GET_FORCEPLOTDATA_HEX);
    BUFF_TX_APPEND_BYTE(1);
    BUFF_TX_APPEND_BYTE(32);
    BUFF_TX_END();
}
static int csafe_pm_get_forceplotdata_resp(csafe_buffer_t *b, csafe_data_t *o) {
    unsigned char len;
    unsigned char n;

    BUFF_RX_INIT(CSAFE_SETUSERCFG1_CMD_HEX, (MAX_FORCEPLOTDATA_BYTES+3));
    BUFF_RX_PM3_CHK(CSAFE_PM_GET_FORCEPLOTDATA_HEX, (MAX_FORCEPLOTDATA_BYTES+1));

    BUFF_RX_POP_BYTE(len);
    len = len > MAX_FORCEPLOTDATA_BYTES ? 0 : len;

    for (n = 0; n < len / 2; n++) {
        BUFF_RX_POP_VAL(o->force_plot_data[i], 2);
    }
    BUFF_RX_INC(MAX_FORCEPLOTDATA_BYTES - len);
    BUFF_RX_END();
}
static int csafe_pm_set_screenerrormode_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETUSERCFG1_CMD_HEX);
    BUFF_TX_APPEND_BYTE(2);
    BUFF_TX_APPEND_BYTE(CSAFE_PM_SET_SCREENERRORMODE_HEX);
    BUFF_TX_APPEND_BYTE(o->setscreenerrormode);
    BUFF_TX_END();
}
static int csafe_pm_set_screenerrormode_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return 0;
}
static int csafe_pm_get_heartbeatdata_req(csafe_buffer_t *b, csafe_data_t *o) {
    BUFF_TX_INIT(CSAFE_SETUSERCFG1_CMD_HEX);
    BUFF_TX_APPEND_BYTE(3);
    BUFF_TX_APPEND_BYTE(CSAFE_PM_GET_HEARTBEATDATA_HEX);
    BUFF_TX_APPEND_BYTE(1);
    BUFF_TX_APPEND_BYTE(MAX_HEARTBEATDATA_BYTES);
    BUFF_TX_END();
}
static int csafe_pm_get_heartbeatdata_resp(csafe_buffer_t *b, csafe_data_t *o) {
    unsigned char len;
    unsigned char n;

    BUFF_RX_INIT(CSAFE_SETUSERCFG1_CMD_HEX, (MAX_HEARTBEATDATA_BYTES+3));
    BUFF_RX_PM3_CHK(CSAFE_PM_GET_HEARTBEATDATA_HEX, (MAX_HEARTBEATDATA_BYTES+1));

    BUFF_RX_POP_BYTE(len);
    len = len > MAX_HEARTBEATDATA_BYTES ? 0 : len;

    for (n = 0; n < len / 2; n++) {
        BUFF_RX_POP_VAL(o->force_plot_data[i], 2);
    }
    BUFF_RX_INC(MAX_HEARTBEATDATA_BYTES - len);
    BUFF_RX_END();
}

/*
 * Some commands are PM(3/4/5) specific and exist as
 * extensions to the base CSAFE commands. These occupy
 * the byte _after_ the initial command byte.
 *
 * The handlers for these commands will interrogate the
 * 2nd byte and jump out as per the table below.
 */
static csafe_handler_t csafe_pm3_resp[] = {
    /* 0x00 */ NULL,
    /* 0x01 */ NULL,
    /* 0x02 */ NULL,
    /* 0x03 */ NULL,
    /* 0x04 */ NULL,
    /* 0x05 */ csafe_pm_set_splitduration_resp,
    /* 0x06 */ NULL,
    /* 0x07 */ NULL,
    /* 0x08 */ NULL,
    /* 0x09 */ NULL,
    /* 0x0a */ NULL,
    /* 0x0b */ NULL,
    /* 0x0c */ NULL,
    /* 0x0d */ NULL,
    /* 0x0e */ NULL,
    /* 0x0f */ NULL,
    /* 0x10 */ NULL,
    /* 0x11 */ NULL,
    /* 0x12 */ NULL,
    /* 0x13 */ NULL,
    /* 0x14 */ NULL,
    /* 0x15 */ NULL,
    /* 0x16 */ NULL,
    /* 0x17 */ NULL,
    /* 0x18 */ NULL,
    /* 0x19 */ NULL,
    /* 0x1a */ NULL,
    /* 0x1b */ NULL,
    /* 0x1c */ NULL,
    /* 0x1d */ NULL,
    /* 0x1e */ NULL,
    /* 0x1f */ NULL,
    /* 0x20 */ NULL,
    /* 0x21 */ NULL,
    /* 0x22 */ NULL,
    /* 0x23 */ NULL,
    /* 0x24 */ NULL,
    /* 0x25 */ NULL,
    /* 0x26 */ NULL,
    /* 0x27 */ csafe_pm_set_screenerrormode_resp,
    /* 0x28 */ NULL,
    /* 0x29 */ NULL,
    /* 0x2a */ NULL,
    /* 0x2b */ NULL,
    /* 0x2c */ NULL,
    /* 0x2d */ NULL,
    /* 0x2e */ NULL,
    /* 0x2f */ NULL,
    /* 0x30 */ NULL,
    /* 0x31 */ NULL,
    /* 0x32 */ NULL,
    /* 0x33 */ NULL,
    /* 0x34 */ NULL,
    /* 0x35 */ NULL,
    /* 0x36 */ NULL,
    /* 0x37 */ NULL,
    /* 0x38 */ NULL,
    /* 0x39 */ NULL,
    /* 0x3a */ NULL,
    /* 0x3b */ NULL,
    /* 0x3c */ NULL,
    /* 0x3d */ NULL,
    /* 0x3e */ NULL,
    /* 0x3f */ NULL,
    /* 0x40 */ NULL,
    /* 0x41 */ NULL,
    /* 0x42 */ NULL,
    /* 0x43 */ NULL,
    /* 0x44 */ NULL,
    /* 0x45 */ NULL,
    /* 0x46 */ NULL,
    /* 0x47 */ NULL,
    /* 0x48 */ NULL,
    /* 0x49 */ NULL,
    /* 0x4a */ NULL,
    /* 0x4b */ NULL,
    /* 0x4c */ NULL,
    /* 0x4d */ NULL,
    /* 0x4e */ NULL,
    /* 0x4f */ NULL,
    /* 0x50 */ NULL,
    /* 0x51 */ NULL,
    /* 0x52 */ NULL,
    /* 0x53 */ NULL,
    /* 0x54 */ NULL,
    /* 0x55 */ NULL,
    /* 0x56 */ NULL,
    /* 0x57 */ NULL,
    /* 0x58 */ NULL,
    /* 0x59 */ NULL,
    /* 0x5a */ NULL,
    /* 0x5b */ NULL,
    /* 0x5c */ NULL,
    /* 0x5d */ NULL,
    /* 0x5e */ NULL,
    /* 0x5f */ NULL,
    /* 0x60 */ NULL,
    /* 0x61 */ NULL,
    /* 0x62 */ NULL,
    /* 0x63 */ NULL,
    /* 0x64 */ NULL,
    /* 0x65 */ csafe_gettextstatus_resp,
    /* 0x66 */ NULL,
    /* 0x67 */ NULL,
    /* 0x68 */ NULL,
    /* 0x69 */ NULL,
    /* 0x6a */ NULL,
    /* 0x6b */ csafe_pm_get_forceplotdata_resp,
    /* 0x6c */ csafe_pm_get_heartbeatdata_resp,
    /* 0x6d */ NULL,
    /* 0x6e */ NULL,
    /* 0x6f */ NULL,
    /* 0x70 */ NULL,
    /* 0x71 */ NULL,
    /* 0x72 */ NULL,
    /* 0x73 */ NULL,
    /* 0x74 */ NULL,
    /* 0x75 */ NULL,
    /* 0x76 */ NULL,
    /* 0x77 */ NULL,
    /* 0x78 */ NULL,
    /* 0x79 */ NULL,
    /* 0x7a */ NULL,
    /* 0x7b */ NULL,
    /* 0x7c */ NULL,
    /* 0x7d */ NULL,
    /* 0x7e */ NULL,
    /* 0x7f */ NULL,
    /* 0x80 */ NULL,
    /* 0x81 */ NULL,
    /* 0x82 */ NULL,
    /* 0x83 */ NULL,
    /* 0x84 */ NULL,
    /* 0x85 */ NULL,
    /* 0x86 */ NULL,
    /* 0x87 */ NULL,
    /* 0x88 */ NULL,
    /* 0x89 */ csafe_pm_get_workouttype_resp,
    /* 0x8a */ NULL,
    /* 0x8b */ NULL,
    /* 0x8c */ NULL,
    /* 0x8d */ csafe_pm_get_workoutstate_resp,
    /* 0x8e */ csafe_pm_get_intervaltype_resp,
    /* 0x8f */ NULL,
    /* 0x90 */ NULL,
    /* 0x91 */ NULL,
    /* 0x92 */ NULL,
    /* 0x93 */ NULL,
    /* 0x94 */ NULL,
    /* 0x95 */ NULL,
    /* 0x96 */ NULL,
    /* 0x97 */ NULL,
    /* 0x98 */ NULL,
    /* 0x99 */ NULL,
    /* 0x9a */ NULL,
    /* 0x9b */ NULL,
    /* 0x9c */ NULL,
    /* 0x9d */ NULL,
    /* 0x9e */ NULL,
    /* 0x9f */ csafe_pm_get_workoutintervalcount_resp,
    /* 0xa0 */ csafe_pm_get_worktime_resp,
    /* 0xa1 */ NULL,
    /* 0xa2 */ NULL,
    /* 0xa3 */ csafe_pm_get_workdistance_resp,
    /* 0xa4 */ NULL,
    /* 0xa5 */ NULL,
    /* 0xa6 */ NULL,
    /* 0xa7 */ NULL,
    /* 0xa8 */ NULL,
    /* 0xa9 */ NULL,
    /* 0xaa */ NULL,
    /* 0xab */ NULL,
    /* 0xac */ NULL,
    /* 0xad */ NULL,
    /* 0xae */ NULL,
    /* 0xaf */ NULL,
    /* 0xb0 */ NULL,
    /* 0xb1 */ NULL,
    /* 0xb2 */ NULL,
    /* 0xb3 */ NULL,
    /* 0xb4 */ NULL,
    /* 0xb5 */ NULL,
    /* 0xb6 */ NULL,
    /* 0xb7 */ NULL,
    /* 0xb8 */ NULL,
    /* 0xb9 */ NULL,
    /* 0xba */ NULL,
    /* 0xbb */ NULL,
    /* 0xbc */ NULL,
    /* 0xbd */ NULL,
    /* 0xbe */ NULL,
    /* 0xbf */ csafe_pm_get_strokestate_resp,
    /* 0xc0 */ NULL,
    /* 0xc1 */ csafe_pm_get_dragfactor_resp,
    /* 0xc2 */ NULL,
    /* 0xc3 */ NULL,
    /* 0xc4 */ NULL,
    /* 0xc5 */ NULL,
    /* 0xc6 */ NULL,
    /* 0xc7 */ NULL,
    /* 0xc8 */ NULL,
    /* 0xc9 */ csafe_pm_get_errorvalue_resp,
    /* 0xca */ NULL,
    /* 0xcb */ NULL,
    /* 0xcc */ NULL,
    /* 0xcd */ NULL,
    /* 0xce */ NULL,
    /* 0xcf */ csafe_pm_get_resttime_resp,
    /* 0xd0 */ NULL,
    /* 0xd1 */ NULL,
    /* 0xd2 */ NULL,
    /* 0xd3 */ NULL,
    /* 0xd4 */ NULL,
    /* 0xd5 */ NULL,
    /* 0xd6 */ NULL,
    /* 0xd7 */ NULL,
    /* 0xd8 */ NULL,
    /* 0xd9 */ NULL,
    /* 0xda */ NULL,
    /* 0xdb */ NULL,
    /* 0xdc */ NULL,
    /* 0xdd */ NULL,
    /* 0xde */ NULL,
    /* 0xdf */ NULL,
    /* 0xe0 */ NULL,
    /* 0xe1 */ NULL,
    /* 0xe2 */ NULL,
    /* 0xe3 */ NULL,
    /* 0xe4 */ NULL,
    /* 0xe5 */ NULL,
    /* 0xe6 */ NULL,
    /* 0xe7 */ NULL,
    /* 0xe8 */ NULL,
    /* 0xe9 */ NULL,
    /* 0xea */ NULL,
    /* 0xeb */ NULL,
    /* 0xec */ NULL,
    /* 0xed */ NULL,
    /* 0xee */ NULL,
    /* 0xef */ NULL,
    /* 0xf0 */ NULL,
    /* 0xf1 */ NULL,
    /* 0xf2 */ NULL,
    /* 0xf3 */ NULL,
    /* 0xf4 */ NULL,
    /* 0xf5 */ NULL,
    /* 0xf6 */ NULL,
    /* 0xf7 */ NULL,
    /* 0xf8 */ NULL,
    /* 0xf9 */ NULL,
    /* 0xfa */ NULL,
    /* 0xfb */ NULL,
    /* 0xfc */ NULL,
    /* 0xfd */ NULL,
    /* 0xfe */ NULL,
    /* 0xff */ NULL
};
static int csafe_setusercfg1_req(csafe_buffer_t *b, csafe_data_t *o) {
    return b->index > 2 && csafe_pm3_resp[b->buff[2]] ?
        csafe_pm3_resp[b->buff[2]](b, o) : -1;
}
static int csafe_setusercfg1_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return b->index > 2 && csafe_pm3_resp[b->buff[2]] ?
        csafe_pm3_resp[b->buff[2]](b, o) : -1;
}
static int csafe_getusercfg1_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_getusercfg1_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setusercfg2_req(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}
static int csafe_setusercfg2_resp(csafe_buffer_t *b, csafe_data_t *o) {
    return -1;  /* unimplemented */
}

/*
 * CSAFE requests. These match up with csafe_cmd from csafe.h.
 */
static csafe_handler_t csafe_req[] = {
    /* CSAFE_GETSTATUS_CMD */       csafe_getstatus_req,
    /* CSAFE_RESET_CMD */           csafe_reset_req,
    /* CSAFE_GOIDLE_CMD */          csafe_goidle_req,
    /* CSAFE_GOHAVEID_CMD */        csafe_gohaveid_req,
    /* CSAFE_GOINUSE_CMD */         csafe_goinuse_req,
    /* CSAFE_GOFINISHED_CMD */      csafe_gofinished_req,
    /* CSAFE_GOREADY_CMD */         csafe_goready_req,
    /* CSAFE_BADID_CMD */           csafe_badid_req,
    /* CSAFE_GETVERSION_CMD */      csafe_getversion_req,
    /* CSAFE_GETID_CMD */           csafe_getid_req,
    /* CSAFE_GETUNITS_CMD */        csafe_getunits_req,
    /* CSAFE_GETSERIAL_CMD */       csafe_getserial_req,
    /* CSAFE_GETLIST_CMD */         csafe_getlist_req,
    /* CSAFE_GETUTILIZATION_CMD */  csafe_getutilization_req,
    /* CSAFE_GETMOTORCURRENT_CMD */ csafe_getmotorcurrent_req,
    /* CSAFE_GETODOMETER_CMD */     csafe_getodometer_req,
    /* CSAFE_GETERRORCODE_CMD */    csafe_geterrorcode_req,
    /* CSAFE_GETSERVICECODE_CMD */  csafe_getservicecode_req,
    /* CSAFE_GETUSERCFG1_CMD */     csafe_getusercfg1_req,
    /* CSAFE_GETUSERCFG2_CMD */     csafe_getusercfg2_req,
    /* CSAFE_GETTWORK_CMD */        csafe_gettwork_req,
    /* CSAFE_GETHORIZONTAL_CMD */   csafe_gethorizontal_req,
    /* CSAFE_GETVERTICAL_CMD */     csafe_getvertical_req,
    /* CSAFE_GETCALORIES_CMD */     csafe_getcalories_req,
    /* CSAFE_GETPROGRAM_CMD */      csafe_getprogram_req,
    /* CSAFE_GETSPEED_CMD */        csafe_getspeed_req,
    /* CSAFE_GETPACE_CMD */         csafe_getpace_req,
    /* CSAFE_GETCADENCE_CMD */      csafe_getcadence_req,
    /* CSAFE_GETGRADE_CMD */        csafe_getgrade_req,
    /* CSAFE_GETGEAR_CMD */         csafe_getgear_req,
    /* CSAFE_GETUPLIST_CMD */       csafe_getuplist_req,
    /* CSAFE_GETUSERINFO_CMD */     csafe_getuserinfo_req,
    /* CSAFE_GETTORQUE_CMD */       csafe_gettorque_req,
    /* CSAFE_GETHRCUR_CMD */        csafe_gethrcur_req,
    /* CSAFE_GETHRTZONE_CMD */      csafe_gethrtzone_req,
    /* CSAFE_GETMETS_CMD */         csafe_getmets_req,
    /* CSAFE_GETPOWER_CMD */        csafe_getpower_req,
    /* CSAFE_GETHRAVG_CMD */        csafe_gethravg_req,
    /* CSAFE_GETHRMAX_CMD */        csafe_gethrmax_req,
    /* CSAFE_GETUSERDATA1_CMD */    csafe_getuserdata1_req,
    /* CSAFE_GETUSERDATA2_CMD */    csafe_getuserdata2_req,
    /* CSAFE_GETAUDIOCHANNEL_CMD */ csafe_getaudiochannel_req,
    /* CSAFE_GETAUDIOVOLUME_CMD */  csafe_getaudiovolume_req,
    /* CSAFE_GETAUDIOMUTE_CMD */    csafe_getaudiomute_req,
    /* CSAFE_ENDTEXT_CMD */         csafe_endtext_req,
    /* CSAFE_DISPLAYPOPUP_CMD */    csafe_displaypopup_req,
    /* CSAFE_GETPOPUPSTATUS_CMD */  csafe_getpopupstatus_req,
    /* CSAFE_AUTOUPLOAD_CMD */      csafe_autoupload_req,
    /* CSAFE_UPLIST_CMD */          csafe_uplist_req,
    /* CSAFE_UPSTATUSSEC_CMD */     csafe_upstatussec_req,
    /* CSAFE_UPLISTSEC_CMD */       csafe_uplistsec_req,
    /* CSAFE_IDDIGITS_CMD */        csafe_iddigits_req,
    /* CSAFE_SETTIME_CMD */         csafe_settime_req,
    /* CSAFE_SETDATE_CMD */         csafe_setdate_req,
    /* CSAFE_SETTIMEOUT_CMD */      csafe_settimeout_req,
    /* CSAFE_SETUSERCFG1_CMD */     csafe_setusercfg1_req,
    /* CSAFE_SETUSERCFG2_CMD */     csafe_setusercfg2_req,
    /* CSAFE_SETTWORK_CMD */        csafe_settwork_req,
    /* CSAFE_SETHORIZONTAL_CMD */   csafe_sethorizontal_req,
    /* CSAFE_SETVERTICAL_CMD */     csafe_setvertical_req,
    /* CSAFE_SETCALORIES_CMD */     csafe_setcalories_req,
    /* CSAFE_SETPROGRAM_CMD */      csafe_setprogram_req,
    /* CSAFE_SETSPEED_CMD */        csafe_setspeed_req,
    /* CSAFE_SETGRADE_CMD */        csafe_setgrade_req,
    /* CSAFE_SETGEAR_CMD */         csafe_setgear_req,
    /* CSAFE_SETUSERINFO_CMD */     csafe_setuserinfo_req,
    /* CSAFE_SETTORQUE_CMD */       csafe_settorque_req,
    /* CSAFE_SETLEVEL_CMD */        csafe_setlevel_req,
    /* CSAFE_SETTARGETHR_CMD */     csafe_settargethr_req,
    /* CSAFE_SETMETS_CMD */         csafe_setmets_req,
    /* CSAFE_SETPOWER_CMD */        csafe_setpower_req,
    /* CSAFE_SETHRZONE_CMD */       csafe_sethrzone_req,
    /* CSAFE_SETHRMAX_CMD */        csafe_sethrmax_req,
    /* CSAFE_SETCHANNELRANGE_CMD */ csafe_setchannelrange_req,
    /* CSAFE_SETVOLUMERANGE_CMD */  csafe_setvolumerange_req,
    /* CSAFE_SETAUDIOMUTE_CMD */    csafe_setaudiomute_req,
    /* CSAFE_SETAUDIOCHANNEL_CMD */ csafe_setaudiochannel_req,
    /* CSAFE_SETAUDIOVOLUME_CMD */  csafe_setaudiovolume_req,
    /* CSAFE_STARTTEXT_CMD */       csafe_starttext_req,
    /* CSAFE_APPENDTEXT_CMD */      csafe_appendtext_req,
    /* CSAFE_GETTEXTSTATUS_CMD */   csafe_gettextstatus_req,
    /* CSAFE_GETCAPS_CMD */         csafe_getcaps_req,
    /* CSAFE_SETPMCFG_CMD */        csafe_setpmcfg_req,
    /* CSAFE_SETPMDATA_CMD */       csafe_setpmdata_req,
    /* CSAFE_GETPMCFG_CMD */        csafe_getpmcfg_req,
    /* CSAFE_GETPMDATA_CMD */       csafe_getpmdata_req,

    /* PM3 specific short commands */
    /* CSAFE_PM_GET_WORKOUTTYPE */  csafe_pm_get_workouttype_req,
    /* CSAFE_PM_GET_DRAGFACTOR */   csafe_pm_get_dragfactor_req,
    /* CSAFE_PM_GET_STROKESTATE */  csafe_pm_get_strokestate_req,
    /* CSAFE_PM_GET_WORKTIME */     csafe_pm_get_worktime_req,
    /* CSAFE_PM_GET_WORKDISTANCE */ csafe_pm_get_workdistance_req,
    /* CSAFE_PM_GET_ERRORVALUE */   csafe_pm_get_errorvalue_req,
    /* CSAFE_PM_GET_WORKOUTSTATE */ csafe_pm_get_workoutstate_req,
    /* CSAFE_PM_GET_WORKOUTINTERVALCOUNT */ csafe_pm_get_workoutintervalcount_req,
    /* CSAFE_PM_GET_INTERVALTYPE */ csafe_pm_get_intervaltype_req,
    /* CSAFE_PM_GET_RESTTIME */     csafe_pm_get_resttime_req,

    /* PM3 specific long commands */
    /* CSAFE_PM_SET_SPLITDURATION */ csafe_pm_set_splitduration_req,
    /* CSAFE_PM_GET_FORCEPLOTDATA */ csafe_pm_get_forceplotdata_req,
    /* CSAFE_PM_SET_SCREENERRORMODE */ csafe_pm_set_screenerrormode_req,
    /* CSAFE_PM_GET_HEARTBEATDATA */ csafe_pm_get_heartbeatdata_req
};

/*
 * CSAFE responses.
 */
static csafe_handler_t csafe_resp[] = {
    /* 0x00 */ NULL,
    /* 0x01 */ csafe_autoupload_resp,
    /* 0x02 */ csafe_uplist_resp,
    /* 0x03 */ NULL,
    /* 0x04 */ csafe_upstatussec_resp,
    /* 0x05 */ csafe_uplistsec_resp,
    /* 0x06 */ NULL,
    /* 0x07 */ NULL,
    /* 0x08 */ NULL,
    /* 0x09 */ NULL,
    /* 0x0a */ NULL,
    /* 0x0b */ NULL,
    /* 0x0c */ NULL,
    /* 0x0d */ NULL,
    /* 0x0e */ NULL,
    /* 0x0f */ NULL,
    /* 0x10 */ csafe_iddigits_resp,
    /* 0x11 */ csafe_settime_resp,
    /* 0x12 */ csafe_setdate_resp,
    /* 0x13 */ csafe_settimeout_resp,
    /* 0x14 */ NULL,
    /* 0x15 */ NULL,
    /* 0x16 */ NULL,
    /* 0x17 */ NULL,
    /* 0x18 */ NULL,
    /* 0x19 */ NULL,
    /* 0x1a */ csafe_setusercfg1_resp,
    /* 0x1b */ csafe_setusercfg2_resp,
    /* 0x1c */ NULL,
    /* 0x1d */ NULL,
    /* 0x1e */ NULL,
    /* 0x1f */ NULL,
    /* 0x20 */ csafe_settwork_resp,
    /* 0x21 */ csafe_sethorizontal_resp,
    /* 0x22 */ csafe_setvertical_resp,
    /* 0x23 */ csafe_setcalories_resp,
    /* 0x24 */ csafe_setprogram_resp,
    /* 0x25 */ csafe_setspeed_resp,
    /* 0x26 */ NULL,
    /* 0x27 */ NULL,
    /* 0x28 */ csafe_setgrade_resp,
    /* 0x29 */ csafe_setgear_resp,
    /* 0x2a */ NULL,
    /* 0x2b */ csafe_setuserinfo_resp,
    /* 0x2c */ csafe_settorque_resp,
    /* 0x2d */ csafe_setlevel_resp,
    /* 0x2e */ NULL,
    /* 0x2f */ NULL,
    /* 0x30 */ csafe_settargethr_resp,
    /* 0x31 */ NULL,
    /* 0x32 */ NULL,
    /* 0x33 */ csafe_setmets_resp,
    /* 0x34 */ csafe_setpower_resp,
    /* 0x35 */ csafe_sethrzone_resp,
    /* 0x36 */ csafe_sethrmax_resp,
    /* 0x37 */ NULL,
    /* 0x38 */ NULL,
    /* 0x39 */ NULL,
    /* 0x3a */ NULL,
    /* 0x3b */ NULL,
    /* 0x3c */ NULL,
    /* 0x3d */ NULL,
    /* 0x3e */ NULL,
    /* 0x3f */ NULL,
    /* 0x40 */ csafe_setchannelrange_resp,
    /* 0x41 */ csafe_setvolumerange_resp,
    /* 0x42 */ csafe_setaudiomute_resp,
    /* 0x43 */ csafe_setaudiochannel_resp,
    /* 0x44 */ csafe_setaudiovolume_resp,
    /* 0x45 */ NULL,
    /* 0x46 */ NULL,
    /* 0x47 */ NULL,
    /* 0x48 */ NULL,
    /* 0x49 */ NULL,
    /* 0x4a */ NULL,
    /* 0x4b */ NULL,
    /* 0x4c */ NULL,
    /* 0x4d */ NULL,
    /* 0x4e */ NULL,
    /* 0x4f */ NULL,
    /* 0x50 */ NULL,
    /* 0x51 */ NULL,
    /* 0x52 */ NULL,
    /* 0x53 */ NULL,
    /* 0x54 */ NULL,
    /* 0x55 */ NULL,
    /* 0x56 */ NULL,
    /* 0x57 */ NULL,
    /* 0x58 */ NULL,
    /* 0x59 */ NULL,
    /* 0x5a */ NULL,
    /* 0x5b */ NULL,
    /* 0x5c */ NULL,
    /* 0x5d */ NULL,
    /* 0x5e */ NULL,
    /* 0x5f */ NULL,
    /* 0x60 */ csafe_starttext_resp,
    /* 0x61 */ csafe_appendtext_resp,
    /* 0x62 */ NULL,
    /* 0x63 */ NULL,
    /* 0x64 */ NULL,
    /* 0x65 */ csafe_gettextstatus_resp,
    /* 0x66 */ NULL,
    /* 0x67 */ NULL,
    /* 0x68 */ NULL,
    /* 0x69 */ NULL,
    /* 0x6a */ NULL,
    /* 0x6b */ NULL,
    /* 0x6c */ NULL,
    /* 0x6d */ NULL,
    /* 0x6e */ NULL,
    /* 0x6f */ NULL,
    /* 0x70 */ csafe_getcaps_resp,
    /* 0x71 */ NULL,
    /* 0x72 */ NULL,
    /* 0x73 */ NULL,
    /* 0x74 */ NULL,
    /* 0x75 */ NULL,
    /* 0x76 */ csafe_setpmcfg_resp,
    /* 0x77 */ csafe_setpmdata_resp,
    /* 0x78 */ NULL,
    /* 0x79 */ NULL,
    /* 0x7a */ NULL,
    /* 0x7b */ NULL,
    /* 0x7c */ NULL,
    /* 0x7d */ NULL,
    /* 0x7e */ csafe_getpmcfg_resp,
    /* 0x7f */ csafe_getpmdata_resp,
    /* 0x80 */ csafe_getstatus_resp,
    /* 0x81 */ csafe_reset_resp,
    /* 0x82 */ csafe_goidle_resp,
    /* 0x83 */ csafe_gohaveid_resp,
    /* 0x84 */ NULL,
    /* 0x85 */ csafe_goinuse_resp,
    /* 0x86 */ csafe_gofinished_resp,
    /* 0x87 */ csafe_goready_resp,
    /* 0x88 */ csafe_badid_resp,
    /* 0x89 */ NULL,
    /* 0x8a */ NULL,
    /* 0x8b */ NULL,
    /* 0x8c */ NULL,
    /* 0x8d */ NULL,
    /* 0x8e */ NULL,
    /* 0x8f */ NULL,
    /* 0x90 */ NULL,
    /* 0x91 */ csafe_getversion_resp,
    /* 0x92 */ csafe_getid_resp,
    /* 0x93 */ csafe_getunits_resp,
    /* 0x94 */ csafe_getserial_resp,
    /* 0x95 */ NULL,
    /* 0x96 */ NULL,
    /* 0x97 */ NULL,
    /* 0x98 */ csafe_getlist_resp,
    /* 0x99 */ csafe_getutilization_resp,
    /* 0x9a */ csafe_getmotorcurrent_resp,
    /* 0x9b */ csafe_getodometer_resp,
    /* 0x9c */ csafe_geterrorcode_resp,
    /* 0x9d */ csafe_getservicecode_resp,
    /* 0x9e */ csafe_getusercfg1_resp,
    /* 0x9f */ csafe_getusercfg2_resp,
    /* 0xa0 */ csafe_gettwork_resp,
    /* 0xa1 */ csafe_gethorizontal_resp,
    /* 0xa2 */ csafe_getvertical_resp,
    /* 0xa3 */ csafe_getcalories_resp,
    /* 0xa4 */ csafe_getprogram_resp,
    /* 0xa5 */ csafe_getspeed_resp,
    /* 0xa6 */ csafe_getpace_resp,
    /* 0xa7 */ csafe_getcadence_resp,
    /* 0xa8 */ csafe_getgrade_resp,
    /* 0xa9 */ csafe_getgear_resp,
    /* 0xaa */ csafe_getuplist_resp,
    /* 0xab */ csafe_getuserinfo_resp,
    /* 0xac */ csafe_gettorque_resp,
    /* 0xad */ NULL,
    /* 0xae */ NULL,
    /* 0xaf */ NULL,
    /* 0xb0 */ csafe_gethrcur_resp,
    /* 0xb1 */ NULL,
    /* 0xb2 */ csafe_gethrtzone_resp,
    /* 0xb3 */ csafe_getmets_resp,
    /* 0xb4 */ csafe_getpower_resp,
    /* 0xb5 */ csafe_gethravg_resp,
    /* 0xb6 */ csafe_gethrmax_resp,
    /* 0xb7 */ NULL,
    /* 0xb8 */ NULL,
    /* 0xb9 */ NULL,
    /* 0xba */ NULL,
    /* 0xbb */ NULL,
    /* 0xbc */ NULL,
    /* 0xbd */ NULL,
    /* 0xbe */ csafe_getuserdata1_resp,
    /* 0xbf */ csafe_getuserdata2_resp,
    /* 0xc0 */ csafe_getaudiochannel_resp,
    /* 0xc1 */ csafe_getaudiovolume_resp,
    /* 0xc2 */ csafe_getaudiomute_resp,
    /* 0xc3 */ NULL,
    /* 0xc4 */ NULL,
    /* 0xc5 */ NULL,
    /* 0xc6 */ NULL,
    /* 0xc7 */ NULL,
    /* 0xc8 */ NULL,
    /* 0xc9 */ NULL,
    /* 0xca */ NULL,
    /* 0xcb */ NULL,
    /* 0xcc */ NULL,
    /* 0xcd */ NULL,
    /* 0xce */ NULL,
    /* 0xcf */ NULL,
    /* 0xd0 */ NULL,
    /* 0xd1 */ NULL,
    /* 0xd2 */ NULL,
    /* 0xd3 */ NULL,
    /* 0xd4 */ NULL,
    /* 0xd5 */ NULL,
    /* 0xd6 */ NULL,
    /* 0xd7 */ NULL,
    /* 0xd8 */ NULL,
    /* 0xd9 */ NULL,
    /* 0xda */ NULL,
    /* 0xdb */ NULL,
    /* 0xdc */ NULL,
    /* 0xdd */ NULL,
    /* 0xde */ NULL,
    /* 0xdf */ NULL,
    /* 0xe0 */ csafe_endtext_resp,
    /* 0xe1 */ csafe_displaypopup_resp,
    /* 0xe2 */ NULL,
    /* 0xe3 */ NULL,
    /* 0xe4 */ NULL,
    /* 0xe5 */ csafe_getpopupstatus_resp,
    /* 0xe6 */ NULL,
    /* 0xe7 */ NULL,
    /* 0xe8 */ NULL,
    /* 0xe9 */ NULL,
    /* 0xea */ NULL,
    /* 0xeb */ NULL,
    /* 0xec */ NULL,
    /* 0xed */ NULL,
    /* 0xee */ NULL,
    /* 0xef */ NULL,
    /* 0xf0 */ NULL,
    /* 0xf1 */ NULL,
    /* 0xf2 */ NULL,
    /* 0xf3 */ NULL,
    /* 0xf4 */ NULL,
    /* 0xf5 */ NULL,
    /* 0xf6 */ NULL,
    /* 0xf7 */ NULL,
    /* 0xf8 */ NULL,
    /* 0xf9 */ NULL,
    /* 0xfa */ NULL,
    /* 0xfb */ NULL,
    /* 0xfc */ NULL,
    /* 0xfd */ NULL,
    /* 0xfe */ NULL,
    /* 0xff */ NULL
};

/*
 * Initialise a csafe_data_t structure.
 *
 * Arguments:
 * - pointer to csafe_data_t.
 *
 * Returns:
 * - nothing.
 */
void csafe_init_data(csafe_data_t *o) {
    if (o) {
        memset(o, 0, sizeof(csafe_data_t));
    }
}

/*
 * Initialise a csafe_buffer_t as a standard CSAFE frame.
 *
 * Arguments:
 * - pointer to csafe_buffer_t.
 *
 * Returns:
 * - number of bytes in the buffer.
 */
int csafe_init_standard_cmd(csafe_buffer_t *b) {
    memset(b->buff, 0, sizeof(b->buff));

    b->buff[0] = FRAME_START_STANDARD;
    b->index = 1;

    return b->index;
}

/*
 * Initialise a csafe_buffer_t as an extended CSAFE frame.
 *
 * Arguments:
 * - pointer to csafe_buffer_t.
 *
 * Returns:
 * - number of bytes in the buffer.
 */
int csafe_init_extended_cmd(csafe_buffer_t *b) {
    memset(b->buff, 0, sizeof(b->buff));

    b->buff[0] = FRAME_START_EXTENDED;
    b->index = 1;

    return b->index;
}

/*
 * Add a csafe command to csafe_buffer_t b. Arguments to the
 * command are supplied in csafe_data_t o.
 *
 * Arguments:
 * - command to push.
 * - pointer to csafe_buffer_t.
 * - arguments for command.
 *
 * Returns:
 * - CSAFE_ERR_PUSH_FAILED if command couldn't be completed.
 * - CSAFE_ERR_PUSH_WOULD_EXCEED_BUFFLEN if the resultant
 *   command buffer would exceed available bytes in csafe_buffer_t b.
 * - CSAFE_ERR_CANT_BUILD_COMMAND if no builder for requested
 *   command exists.
 * - number of bytes in the buffer on success.
 */
int csafe_push_cmd(csafe_cmd c, csafe_buffer_t *b, csafe_data_t *o) {
    csafe_buffer_t t;

    if (csafe_req[c]) {
        /*
         * Build new command into temporary buffer and
         * escape it. This gives us the length of the
         * command, and we can check to make sure it fits
         * into the frame.
         */
        if (csafe_req[c](&t, o) == -1) {
            error("%s: no req handler for 0x%02x", __FUNCTION__, c);
            return CSAFE_ERR_PUSH_FAILED;
        }
        byte_stuff(&t);

        /*
         * Make sure existing command + new command doesn't
         * overrun buffer.
         * - minus 1 for checksum
         * - minus 1 for stop flag
         */
        if (b->index + t.index < CSAFE_MAX_FRAME_SIZE - 1 - 1) {
            memcpy(&b->buff[b->index], t.buff, t.index);
            b->index += t.index;
            return b->index;
        }
        error("%s: not enough space in packet (%02x)", __FUNCTION__, t.buff[0]);
        return CSAFE_ERR_PUSH_WOULD_EXCEED_BUFFLEN;
    }

    error("%s: no request builder for command %d", __FUNCTION__, (int)c);
    return CSAFE_ERR_CANT_BUILD_COMMAND;
}

static unsigned char csafe_checksum(unsigned char *b, int len) {
    unsigned char cksum = 0;
    while (len--) {
        cksum ^= *b++;
    }
    return cksum;
}

/*
 * Terminate a CSAFE frame.
 *
 * Arguments:
 * - pointer to csafe_buffer_t.
 *
 * Returns:
 * - CSAFE_ERR_PUSH_WOULD_EXCEED_BUFFLEN if the resultant
 *   command buffer would exceed available bytes in csafe_buffer_t b.
 * - number of bytes in buffer on success.
 */
int csafe_end_cmd(csafe_buffer_t *b) {
    /*
     * minus 1 for checksum
     * minus 1 for stop flag
     */
    if (b->index < CSAFE_MAX_FRAME_SIZE - 1 - 1) {
        b->buff[b->index++] = csafe_checksum(&b->buff[1], b->index-1);
        b->buff[b->index++] = FRAME_STOP;

        return b->index;
    }

    error("%s: not enough room for cksum and stop bytes (%02x)", __FUNCTION__, b->buff[0]);

    return CSAFE_ERR_PUSH_WOULD_EXCEED_BUFFLEN;
}

static int csafe_pop_resp(csafe_buffer_t *b, csafe_data_t *o) {
    int len;

    warning("%s", __FUNCTION__);

    /*
     * If we don't have a response handler, we don't know
     * how many bytes to jump to get to the next response.
     * Cancel processing.
     */
    if (!csafe_resp[b->buff[0]]) {
        error("%s: no response handler for %02x", __FUNCTION__, b->buff[0]);
        return CSAFE_ERR_NO_POP_HANDLER;
    }

    len = csafe_resp[b->buff[0]](b, o);

    if (len < 0) {
        error("%s: failed response handler %02x", __FUNCTION__, b->buff[0]);
        return CSAFE_ERR_POP_FAILED;
    }

    memmove(b->buff, b->buff + len, b->index - len);    /* pop */
    b->index -= len;

    return b->index;
}

/*
 * Unpack the contents of a csafe_buffer_t into a csafe_data_t
 * structure.
 *
 * Arguments:
 * - pointer to csafe_buffer_t.
 * - pointer to csafe_data_t.
 *
 * Returns:
 * - CSAFE_ERR_INVALID_FRAME_START on an invalid frame.
 * - CSAFE_ERR_FAILED_CHECKSUM when the checksum in the frame
 *   doesn't match the checksum calculated.
 * - CSAFE_ERR_POP_FAILED when we are unable to pop a command
 *   from the buffer.
 * - zero on success.
 */
int csafe_unpack(csafe_buffer_t *b, csafe_data_t *o) {
    int i;

    if (b->buff[0] != FRAME_START_STANDARD) {       /* bogus frame */
        warning("%s: failed to get frame start (got %02x)", __FUNCTION__, b->buff[0]);
        return CSAFE_ERR_INVALID_FRAME_START;
    }

    /*
     * - minus 3 for start, checksum and stop bytes.
     * - minus 2 for position of checksum.
     */
    if (csafe_checksum(&b->buff[1], b->index-3) != b->buff[b->index-2]) {
        warning("%s: bad checksum (want %02x, got %02x)",
                __FUNCTION__,
                csafe_checksum(&b->buff[1], b->index-3),
                b->buff[b->index-2]);

        return CSAFE_ERR_FAILED_CHECKSUM;
    }

    byte_unstuff(b);                            /* get rid of escapes */
    strip_start_cksum_stop(b);                  /* we don't need these */

    o->status = b->buff[0] & 0x7f;              /* status byte on all responses */
    memmove(b->buff, b->buff+1, b->index-1);    /* strip status byte */
    b->index--;

#ifdef DEBUG
    dump_status(o);
#endif /* DEBUG */

    if (b->index > 0) {
        for (i = 1; csafe_pop_resp(b, o) > 0; i++);
    }

    return b->index > 0 ? CSAFE_ERR_POP_FAILED : 0;
}

#ifdef UNITTEST_CSAFE

int main(int argc, char *argv[]) {
    return 0;
}

#endif /* UNITTEST_CSAFE */
