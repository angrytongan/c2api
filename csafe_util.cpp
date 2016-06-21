#include <stdio.h>
#include <string.h>

#include "csafe.h"
#include "utils.h"

const char* csafe_cmd_str[] = {
    "CSAFE_GETSTATUS_CMD",
    "CSAFE_RESET_CMD",
    "CSAFE_GOIDLE_CMD",
    "CSAFE_GOHAVEID_CMD",
    "CSAFE_GOINUSE_CMD",
    "CSAFE_GOFINISHED_CMD",
    "CSAFE_GOREADY_CMD",
    "CSAFE_BADID_CMD",
    "CSAFE_GETVERSION_CMD",
    "CSAFE_GETID_CMD",
    "CSAFE_GETUNITS_CMD",
    "CSAFE_GETSERIAL_CMD",
    "CSAFE_GETLIST_CMD",
    "CSAFE_GETUTILIZATION_CMD",
    "CSAFE_GETMOTORCURRENT_CMD",
    "CSAFE_GETODOMETER_CMD",
    "CSAFE_GETERRORCODE_CMD",
    "CSAFE_GETSERVICECODE_CMD",
    "CSAFE_GETUSERCFG1_CMD",
    "CSAFE_GETUSERCFG2_CMD",
    "CSAFE_GETTWORK_CMD",
    "CSAFE_GETHORIZONTAL_CMD",
    "CSAFE_GETVERTICAL_CMD",
    "CSAFE_GETCALORIES_CMD",
    "CSAFE_GETPROGRAM_CMD",
    "CSAFE_GETSPEED_CMD",
    "CSAFE_GETPACE_CMD",
    "CSAFE_GETCADENCE_CMD",
    "CSAFE_GETGRADE_CMD",
    "CSAFE_GETGEAR_CMD",
    "CSAFE_GETUPLIST_CMD",
    "CSAFE_GETUSERINFO_CMD",
    "CSAFE_GETTORQUE_CMD",
    "CSAFE_GETHRCUR_CMD",
    "CSAFE_GETHRTZONE_CMD",
    "CSAFE_GETMETS_CMD",
    "CSAFE_GETPOWER_CMD",
    "CSAFE_GETHRAVG_CMD",
    "CSAFE_GETHRMAX_CMD",
    "CSAFE_GETUSERDATA1_CMD",
    "CSAFE_GETUSERDATA2_CMD",
    "CSAFE_GETAUDIOCHANNEL_CMD",
    "CSAFE_GETAUDIOVOLUME_CMD",
    "CSAFE_GETAUDIOMUTE_CMD",
    "CSAFE_ENDTEXT_CMD",
    "CSAFE_DISPLAYPOPUP_CMD",
    "CSAFE_GETPOPUPSTATUS_CMD",

    /* long commands */
    "CSAFE_AUTOUPLOAD_CMD",
    "CSAFE_UPLIST_CMD",
    "CSAFE_UPSTATUSSEC_CMD",
    "CSAFE_UPLISTSEC_CMD",
    "CSAFE_IDDIGITS_CMD",
    "CSAFE_SETTIME_CMD",
    "CSAFE_SETDATE_CMD",
    "CSAFE_SETTIMEOUT_CMD",
    "CSAFE_SETUSERCFG1_CMD",
    "CSAFE_SETUSERCFG2_CMD",
    "CSAFE_SETTWORK_CMD",
    "CSAFE_SETHORIZONTAL_CMD",
    "CSAFE_SETVERTICAL_CMD",
    "CSAFE_SETCALORIES_CMD",
    "CSAFE_SETPROGRAM_CMD",
    "CSAFE_SETSPEED_CMD",
    "CSAFE_SETGRADE_CMD",
    "CSAFE_SETGEAR_CMD",
    "CSAFE_SETUSERINFO_CMD",
    "CSAFE_SETTORQUE_CMD",
    "CSAFE_SETLEVEL_CMD",
    "CSAFE_SETTARGETHR_CMD",
    "CSAFE_SETMETS_CMD",
    "CSAFE_SETPOWER_CMD",
    "CSAFE_SETHRZONE_CMD",
    "CSAFE_SETHRMAX_CMD",
    "CSAFE_SETCHANNELRANGE_CMD",
    "CSAFE_SETVOLUMERANGE_CMD",
    "CSAFE_SETAUDIOMUTE_CMD",
    "CSAFE_SETAUDIOCHANNEL_CMD",
    "CSAFE_SETAUDIOVOLUME_CMD",
    "CSAFE_STARTTEXT_CMD",
    "CSAFE_APPENDTEXT_CMD",
    "CSAFE_GETTEXTSTATUS_CMD",
    "CSAFE_GETCAPS_CMD",
    "CSAFE_SETPMCFG_CMD",
    "CSAFE_SETPMDATA_CMD",
    "CSAFE_GETPMCFG_CMD",
    "CSAFE_GETPMDATA_CMD",

    /* pm specific short commands */
    "CSAFE_PM_GET_WORKOUTTYPE",
    "CSAFE_PM_GET_DRAGFACTOR",
    "CSAFE_PM_GET_STROKESTATE",
    "CSAFE_PM_GET_WORKTIME",
    "CSAFE_PM_GET_WORKDISTANCE",
    "CSAFE_PM_GET_ERRORVALUE",
    "CSAFE_PM_GET_WORKOUTSTATE",
    "CSAFE_PM_GET_WORKOUTINTERVALCOUNT",
    "CSAFE_PM_GET_INTERVALTYPE",
    "CSAFE_PM_GET_RESTTIME",

    /* pm specific long commands */
    "CSAFE_PM_SET_SPLITDURATION",
    "CSAFE_PM_GET_FORCEPLOTDATA",
    "CSAFE_PM_SET_SCREENERRORMODE",
    "CSAFE_PM_GET_HEARTBEATDATA"
};

const char* csafe_stroke_state[] = {
    "STROKE_STATE_WAITING_MIN_SPEED",
    "STROKE_STATE_WAITING_ACCELERATE",
    "STROKE_STATE_DRIVING",
    "STROKE_STATE_DWELLING",
    "STROKE_STATE_RECOVERY"
};

const char* csafe_workout_type[] = {
    "WORKOUT_TYPE_JUST_ROW_NO_SPLITS",
    "WORKOUT_TYPE_JUST_ROW_SPLITS",
    "WORKOUT_TYPE_FIXED_DISTANCE_NO_SPLITS",
    "WORKOUT_TYPE_FIXED_DISTANCE_SPLITS",
    "WORKOUT_TYPE_FIXED_TIME_NO_SPLITS",
    "WORKOUT_TYPE_FIXED_TIME_SPLITS",
    "WORKOUT_TYPE_FIXED_TIME_INTERVAL",
    "WORKOUT_TYPE_FIXED_DISTANCE_INTERVAL",
    "WORKOUT_TYPE_VARIABLE_INTERVAL"
};

const char* csafe_workout_state[] = {
    "WORKOUT_STATE_WAITING_TO_BEGIN",
    "WORKOUT_STATE_WORKOUT_ROW",
    "WORKOUT_STATE_COUNTDOWN_PAUSE",
    "WORKOUT_STATE_INTERVAL_REST",
    "WORKOUT_STATE_WORK_TIME_INTERVAL",
    "WORKOUT_STATE_WORK_DISTANCE_INTERVAL",
    "WORKOUT_STATE_REST_INTERVAL_END_TO_WORK_TIME_INTERVAL_BEGIN",
    "WORKOUT_STATE_REST_INTERVAL_END_TO_WORK_DISTANCE_INTERVAL_BEGIN",
    "WORKOUT_STATE_WORK_TIME_INTERVAL_END_TO_REST_INTERVAL_BEGIN",
    "WORKOUT_STATE_WORK_DISTANCE_INTERVAL_END_TO_REST_INTERVAL_BEGIN",
    "WORKOUT_STATE_WORKOUT_END",
    "WORKOUT_STATE_WORKOUT_TERMINATE",
    "WORKOUT_STATE_WORKOUT_LOGGED",
    "WORKOUT_STATE_WORKOUT_REARM"
};

const char* csafe_interval_type[] = {
    "INTERVAL_TYPE_TIME",
    "INTERVAL_TYPE_DISTANCE",
    "INTERVAL_TYPE_REST"
};

const char* csafe_machine_state_str[] = {
    "CSAFE_MACHINE_STATE_ERROR",
    "CSAFE_MACHINE_STATE_READY",
    "CSAFE_MACHINE_STATE_IDLE",
    "CSAFE_MACHINE_STATE_HAVEID",
    "CSAFE_MACHINE_STATE_UNASSIGNED",
    "CSAFE_MACHINE_STATE_INUSE",
    "CSAFE_MACHINE_STATE_PAUSED",
    "CSAFE_MACHINE_STATE_FINISHED",
    "CSAFE_MACHINE_STATE_MANUAL",
    "CSAFE_MACHINE_STATE_OFFLINE",
};

const char* csafe_machine_state(int status) {
    return status >= CSAFE_MACHINE_STATE_ERROR &&
        status <= CSAFE_MACHINE_STATE_OFFLINE ?
        csafe_machine_state_str[status] : "unknown state";
}

void dump_status(csafe_data_t *o) {
    printf("Status: %-30s (0x%02x)\n", csafe_machine_state(o->status), o->status);
    printf("Last error: %d (0x%02x)\n", o->error, o->error);
    printf("Error value: %d (0x%02x)\n", o->error_value, o->error_value);
}

void dump_monitor(csafe_data_t *o) {
    unsigned int d, h, m, s;

    printf("twork: %02d:%02d:%02d\n",
            o->twork_hours, o->twork_minutes, o->twork_seconds);
    printf("Horizontal distance: %d (units 0x%02x)\n",
            o->horizontal, o->horizontal_units);

    dhms(o->seconds_per_km, &d, &h, &m, &s);
    printf("Pace: %dsecs/km %d:%02d:%02d:%02d/km\n",
            o->seconds_per_km, d, h, m, s);

    dhms(o->seconds_per_km / 2, &d, &h, &m, &s);
    printf("Pace: %d:%02d:%02d:%02d/500m\n", d, h, m, s);

    printf("Watts: %dW\n", o->watts);
    printf("Calories: %dcal\n", o->calories);
    printf("Stroke rate: %d strokes (units 0x%02x)\n",
            o->cadence, o->cadence_units);
    printf("Current heart rate: %dbpm\n", o->hr);

}

void dump_forceplot(csafe_data_t *o) {
    int i;

    printf("Force plot data: ");
    for (i = 0; i < MAX_FORCEPLOTDATA_BYTES/2; i++) {
        printf("%02d,", o->force_plot_data[i]);
    }
    printf("\n");

    printf("Heart beat data: ");
    for (i = 0; i < MAX_HEARTBEATDATA_BYTES/2; i++) {
        printf("%02d,", o->heart_beat_data[i]);
    }
    printf("\n");

}

void dump_workout(csafe_data_t *o) {
    unsigned int d, h, m, s;

    dhms(o->work_time, &d, &h, &m, &s);
    printf("Work time: %d:%02d:%02d:%02d\n", d, h, m, s);
    printf("Work distance: %dm\n", o->work_distance);
    printf("Stroke state: %s\n", csafe_stroke_state[o->stroke_state]);
    printf("Workout type: %s\n", csafe_workout_type[o->workout_type]);
    printf("Workout state: %s\n", csafe_workout_state[o->workout_state]);
    printf("Drag Factor: %d\n", o->drag_factor);

    printf("Rest time: %d\n", o->rest_time);
    printf("Interval type: %s\n", csafe_interval_type[o->interval_type]);
    printf("Interval count: %d\n", o->workout_interval_count);

    printf("Units (?): %d\n", o->units);
}

void dump_erg(csafe_data_t *o) {
    char serial[10] = { 0 };

    memcpy(serial, o->serial, 9);

    printf("Odometer: %d (%d 0x%02x)\n", o->odometer, o->odometer_units,
            o->odometer_units);
    printf("Manufacturing ID: %d\n", o->mfg_id);
    printf("CID: %d\n", o->cid);
    printf("Model: %d\n", o->model);
    printf("Hardware version: %d\n", o->hw_version);
    printf("Software version: %d\n", o->sw_version);
    printf("Serial: %s\n", o->serial);
}

void dump_csafe_data(csafe_data_t *o) {
     dump_monitor(o);
     dump_forceplot(o);
     dump_workout(o);
     dump_erg(o);
}
