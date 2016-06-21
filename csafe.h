#ifndef CSAFE_H
#define CSAFE_H

/*
 * the maximum frame size in the C2 spec is 96 bytes,
 * but HID seems to work best with a report id of 2
 * and a frame size of 120.
 *
 * XXX Figure this out.
 */
#define CSAFE_MAX_FRAME_SIZE    120

#define CSAFE_ERR_PUSH_FAILED               -1
#define CSAFE_ERR_PUSH_WOULD_EXCEED_BUFFLEN -2
#define CSAFE_ERR_CANT_BUILD_COMMAND        -3
#define CSAFE_ERR_NO_POP_HANDLER            -4
#define CSAFE_ERR_POP_FAILED                -5
#define CSAFE_ERR_INVALID_FRAME_START       -6
#define CSAFE_ERR_FAILED_CHECKSUM           -7

/*
 * Maximum bytes returned from PM for forceplot
 * and heart rate data bytes.
 */
#define MAX_HEARTBEATDATA_BYTES     32
#define MAX_FORCEPLOTDATA_BYTES    32
typedef struct csafe_buffer_st {
    unsigned char   buff[CSAFE_MAX_FRAME_SIZE];
    unsigned int    index;  /* bytes consumed by command */

} csafe_buffer_t;

typedef enum {
    CSAFE_GETSTATUS_CMD = 0,
    CSAFE_RESET_CMD,
    CSAFE_GOIDLE_CMD,
    CSAFE_GOHAVEID_CMD,
    CSAFE_GOINUSE_CMD,
    CSAFE_GOFINISHED_CMD,
    CSAFE_GOREADY_CMD,
    CSAFE_BADID_CMD,
    CSAFE_GETVERSION_CMD,
    CSAFE_GETID_CMD,
    CSAFE_GETUNITS_CMD,
    CSAFE_GETSERIAL_CMD,
    CSAFE_GETLIST_CMD,                  /* unimplemented */
    CSAFE_GETUTILIZATION_CMD,           /* unimplemented */
    CSAFE_GETMOTORCURRENT_CMD,          /* unimplemented */
    CSAFE_GETODOMETER_CMD,
    CSAFE_GETERRORCODE_CMD,
    CSAFE_GETSERVICECODE_CMD,           /* unimplemented */
    CSAFE_GETUSERCFG1_CMD,              /* unimplemented */
    CSAFE_GETUSERCFG2_CMD,              /* unimplemented */
    CSAFE_GETTWORK_CMD,
    CSAFE_GETHORIZONTAL_CMD,
    CSAFE_GETVERTICAL_CMD,              /* unimplemented */
    CSAFE_GETCALORIES_CMD,
    CSAFE_GETPROGRAM_CMD,
    CSAFE_GETSPEED_CMD,                 /* unimplemented */
    CSAFE_GETPACE_CMD,
    CSAFE_GETCADENCE_CMD,
    CSAFE_GETGRADE_CMD,                 /* unimplemented */
    CSAFE_GETGEAR_CMD,                  /* unimplemented */
    CSAFE_GETUPLIST_CMD,                /* unimplemented */
    CSAFE_GETUSERINFO_CMD,
    CSAFE_GETTORQUE_CMD,                /* unimplemented */
    CSAFE_GETHRCUR_CMD,
    CSAFE_GETHRTZONE_CMD,               /* unimplemented */
    CSAFE_GETMETS_CMD,                  /* unimplemented */
    CSAFE_GETPOWER_CMD,
    CSAFE_GETHRAVG_CMD,                 /* unimplemented */
    CSAFE_GETHRMAX_CMD,                 /* unimplemented */
    CSAFE_GETUSERDATA1_CMD,             /* unimplemented */
    CSAFE_GETUSERDATA2_CMD,             /* unimplemented */
    CSAFE_GETAUDIOCHANNEL_CMD,          /* unimplemented */
    CSAFE_GETAUDIOVOLUME_CMD,           /* unimplemented */
    CSAFE_GETAUDIOMUTE_CMD,             /* unimplemented */
    CSAFE_ENDTEXT_CMD,                  /* unimplemented */
    CSAFE_DISPLAYPOPUP_CMD,             /* unimplemented */
    CSAFE_GETPOPUPSTATUS_CMD,           /* unimplemented */

    /* long commands */
    CSAFE_AUTOUPLOAD_CMD,
    CSAFE_UPLIST_CMD,                   /* unimplemented */
    CSAFE_UPSTATUSSEC_CMD,              /* unimplemented */
    CSAFE_UPLISTSEC_CMD,                /* unimplemented */
    CSAFE_IDDIGITS_CMD,
    CSAFE_SETTIME_CMD,
    CSAFE_SETDATE_CMD,
    CSAFE_SETTIMEOUT_CMD,
    CSAFE_SETUSERCFG1_CMD,
    CSAFE_SETUSERCFG2_CMD,              /* unimplemented */
    CSAFE_SETTWORK_CMD,
    CSAFE_SETHORIZONTAL_CMD,
    CSAFE_SETVERTICAL_CMD,              /* unimplemented */
    CSAFE_SETCALORIES_CMD,
    CSAFE_SETPROGRAM_CMD,
    CSAFE_SETSPEED_CMD,                 /* unimplemented */
    CSAFE_SETGRADE_CMD,                 /* unimplemented */
    CSAFE_SETGEAR_CMD,                  /* unimplemented */
    CSAFE_SETUSERINFO_CMD,              /* unimplemented */
    CSAFE_SETTORQUE_CMD,                /* unimplemented */
    CSAFE_SETLEVEL_CMD,                 /* unimplemented */
    CSAFE_SETTARGETHR_CMD,              /* unimplemented */
    CSAFE_SETMETS_CMD,                  /* unimplemented */
    CSAFE_SETPOWER_CMD,
    CSAFE_SETHRZONE_CMD,                /* unimplemented */
    CSAFE_SETHRMAX_CMD,                 /* unimplemented */
    CSAFE_SETCHANNELRANGE_CMD,          /* unimplemented */
    CSAFE_SETVOLUMERANGE_CMD,           /* unimplemented */
    CSAFE_SETAUDIOMUTE_CMD,             /* unimplemented */
    CSAFE_SETAUDIOCHANNEL_CMD,          /* unimplemented */
    CSAFE_SETAUDIOVOLUME_CMD,           /* unimplemented */
    CSAFE_STARTTEXT_CMD,                /* unimplemented */
    CSAFE_APPENDTEXT_CMD,               /* unimplemented */
    CSAFE_GETTEXTSTATUS_CMD,            /* unimplemented */
    CSAFE_GETCAPS_CMD,
    CSAFE_SETPMCFG_CMD,             /* not for public use */
    CSAFE_SETPMDATA_CMD,            /* not for public use */
    CSAFE_GETPMCFG_CMD,             /* not for public use */
    CSAFE_GETPMDATA_CMD,            /* not for public use */

    /* pm specific short commands */
    CSAFE_PM_GET_WORKOUTTYPE,
    CSAFE_PM_GET_DRAGFACTOR,
    CSAFE_PM_GET_STROKESTATE,
    CSAFE_PM_GET_WORKTIME,
    CSAFE_PM_GET_WORKDISTANCE,
    CSAFE_PM_GET_ERRORVALUE,
    CSAFE_PM_GET_WORKOUTSTATE,
    CSAFE_PM_GET_WORKOUTINTERVALCOUNT,
    CSAFE_PM_GET_INTERVALTYPE,
    CSAFE_PM_GET_RESTTIME,

    /* pm specific long commands */
    CSAFE_PM_SET_SPLITDURATION,
    CSAFE_PM_GET_FORCEPLOTDATA,
    CSAFE_PM_SET_SCREENERRORMODE,
    CSAFE_PM_GET_HEARTBEATDATA
} csafe_cmd;

/*
 * CSAFE_SETPROGRAM_CMD workout numbers
 */
#define WORKOUT_PROGRAMMED      0
#define WORKOUT_STANDARD_1      1
#define WORKOUT_STANDARD_2      2
#define WORKOUT_STANDARD_3      3
#define WORKOUT_STANDARD_4      4
#define WORKOUT_STANDARD_5      5
#define WORKOUT_CUSTOM_1        6
#define WORKOUT_CUSTOM_2        7
#define WORKOUT_CUSTOM_3        8
#define WORKOUT_CUSTOM_4        9
#define WORKOUT_CUSTOM_5        10
#define WORKOUT_FAVOURITES_1    11
#define WORKOUT_FAVOURITES_2    12
#define WORKOUT_FAVOURITES_3    13
#define WORKOUT_FAVOURITES_4    14
#define WORKOUT_FAVOURITES_5    15

/*
 * Response from CSAFE_PM_GET_STROKESTATE
 */
#define STROKE_STATE_WAITING_MIN_SPEED  0
#define STROKE_STATE_WAITING_ACCELERATE 1
#define STROKE_STATE_DRIVING            2
#define STROKE_STATE_DWELLING           3
#define STROKE_STATE_RECOVERY           4

/*
 * Response from CSAFE_PM_GET_WORKOUTTYPE
 */
#define WORKOUT_TYPE_JUST_ROW_NO_SPLITS         0
#define WORKOUT_TYPE_JUST_ROW_SPLITS            1
#define WORKOUT_TYPE_FIXED_DISTANCE_NO_SPLITS   2
#define WORKOUT_TYPE_FIXED_DISTANCE_SPLITS      3
#define WORKOUT_TYPE_FIXED_TIME_NO_SPLITS       4
#define WORKOUT_TYPE_FIXED_TIME_SPLITS          5
#define WORKOUT_TYPE_FIXED_TIME_INTERVAL        6
#define WORKOUT_TYPE_FIXED_DISTANCE_INTERVAL    7
#define WORKOUT_TYPE_VARIABLE_INTERVAL          8

/*
 * Response from CSAFE_PM_GET_WORKOUTSTATE
 */
#define WORKOUT_STATE_WAITING_TO_BEGIN          0
#define WORKOUT_STATE_WORKOUT_ROW               1
#define WORKOUT_STATE_COUNTDOWN_PAUSE           2
#define WORKOUT_STATE_INTERVAL_REST             3
#define WORKOUT_STATE_WORK_TIME_INTERVAL        4
#define WORKOUT_STATE_WORK_DISTANCE_INTERVAL    5
#define WORKOUT_STATE_REST_INTERVAL_END_TO_WORK_TIME_INTERVAL_BEGIN 6
#define WORKOUT_STATE_REST_INTERVAL_END_TO_WORK_DISTANCE_INTERVAL_BEGIN 7
#define WORKOUT_STATE_WORK_TIME_INTERVAL_END_TO_REST_INTERVAL_BEGIN 8
#define WORKOUT_STATE_WORK_DISTANCE_INTERVAL_END_TO_REST_INTERVAL_BEGIN 9
#define WORKOUT_STATE_WORKOUT_END               10
#define WORKOUT_STATE_WORKOUT_TERMINATE         11
#define WORKOUT_STATE_WORKOUT_LOGGED            12
#define WORKOUT_STATE_WORKOUT_REARM             13

/*
 * Response from CSAFE_PM_GET_INTERVALTYPE
 */
#define INTERVAL_TYPE_TIME                      0
#define INTERVAL_TYPE_DISTANCE                  1
#define INTERVAL_TYPE_REST                      2

/*
 * Response from:
 * CSAFE_GOREADY_CMD
 * CSAFE_GOIDLE_CMD
 * CSAFE_GOHAVEID_CMD
 * CSAFE_GOINUSE_CMD
 * CSAFE_GOFINISHED_CMD
 */
#define CSAFE_MACHINE_STATE_ERROR       0
#define CSAFE_MACHINE_STATE_READY       1
#define CSAFE_MACHINE_STATE_IDLE        2
#define CSAFE_MACHINE_STATE_HAVEID      3
#define CSAFE_MACHINE_STATE_UNASSIGNED  4
#define CSAFE_MACHINE_STATE_INUSE       5
#define CSAFE_MACHINE_STATE_PAUSED      6
#define CSAFE_MACHINE_STATE_FINISHED    7
#define CSAFE_MACHINE_STATE_MANUAL      8
#define CSAFE_MACHINE_STATE_OFFLINE     9

/*
 * Push and pull data from this data structure when
 * talking to a rower.
 */
typedef struct csafe_data_st {
    unsigned int status;                /* CSAFE_GETSTATUS_CMD */

    /*
     * Variables for SET commands. We do this
     * here so we don't override GET responses.
     */
    unsigned int settime_hour;          /* CSAFE_SETTIME_CMD */
    unsigned int settime_minute;
    unsigned int settime_second;

    unsigned int setdate_year;          /* CSAFE_SETDATE_CMD */
    unsigned int setdate_month;
    unsigned int setdate_day;

    unsigned char settimeout;           /* CSAFE_SETTIMEOUT_CMD */

    unsigned char settwork_hours;       /* CSAFE_SETTWORK_CMD */
    unsigned char settwork_minutes;
    unsigned char settwork_seconds;

    unsigned int sethorizontal;         /* CSAFE_SETHORIZONTAL_CMD */
    unsigned int sethorizontal_units;

    unsigned char setcalories;          /* CSAFE_SETCALORIES_CMD */

    unsigned int setprogram;            /* CSAFE_SETPROGRAM_CMD */

    unsigned int setpower;              /* CSAFE_SETPOWER_CMD */
    unsigned int setpower_units;

    unsigned int setsplit_duration;     /* CSAFE_SETUSERCFG1_CMD_HEX + CSAFE_PM_SET_SPLITDURATION */
    unsigned int setsplit_duration_type;    /* 0: time, 0x80: distance */

    unsigned char setscreenerrormode;   /* CSAFE_SETUSERCFG1_CMD_HEX + CSAFE_PM_SET_SCREENERRORMODE */

    /*
     * Variables for GET commands.
     */
    unsigned char twork_hours;          /* CSAFE_GETTWORK_CMD */
    unsigned char twork_minutes;
    unsigned char twork_seconds;

    unsigned int horizontal;            /* CSAFE_GETHORIZONTAL_CMD */
    unsigned int horizontal_units;

    unsigned int seconds_per_km;        /* CSAFE_GETPACE_CMD */
    unsigned int watts;                 /* CSAFE_GETPOWER_CMD */
    unsigned int calories;              /* CSAFE_GETCALORIES_CMD */
    unsigned int program;               /* CSAFE_GETPROGRAM_CMD */

    unsigned int pace;                  /* CSAFE_GETPACE_CMD */
    unsigned int pace_units;

    unsigned int cadence;               /* CSAFE_GETCADENCE_CMD */
    unsigned int cadence_units;

    unsigned int weight;                /* CSAFE_GETUSERINFO_CMD */
    unsigned int weight_units;
    unsigned int age;
    unsigned int gender;

    unsigned int hr;                    /* CSAFE_GETHRCUR_CMD */

    unsigned int power;                 /* CSAFE_GETPOWER_CMD */
    unsigned int power_units;

    unsigned int error;                 /* CSAFE_GETERRORCODE_CMD */

    unsigned int odometer;              /* CSAFE_GETODOMETER_CMD */
    unsigned int odometer_units;

    unsigned int units;                 /* CSAFE_GETUNITS_CMD */
    unsigned char serial[10];           /* CSAFE_GETSERIAL_CMD */

    /* CSAFE_GETVERSION_CMD */
    unsigned int mfg_id;
    unsigned int cid;
    unsigned int model;
    unsigned int hw_version;
    unsigned int sw_version;

    unsigned int capability_code;       /* CSAFE_GETCAPS_CMD */

    /* PM3 specific commands; CSAFE_SETUSERCFG1_CMD + ... */
    unsigned int split_duration;        /* CSAFE_PM_GET_SPLITDURATION */

    unsigned int work_time;             /* seconds; CSAFE_PM_GET_WORKTIME */
    unsigned int work_time_frac;

    unsigned int work_distance;         /* metres; CSAFE_PM_GET_WORKDISTANCE */
    unsigned int work_distance_frac;

    unsigned int drag_factor;           /* N-M-sec; CSAFE_PM_GET_DRAGFACTOR */
    unsigned int stroke_state;          /* STROKE_STATE_*; CSAFE_PM_GET_STROKESTATE */
    unsigned int workout_type;          /* WORKOUT_TYPE_*; CSAFE_PM_GET_WORKOUTTYPE */
    unsigned int error_value;           /* CSAFE_PM_GET_ERRORVALUE */

    unsigned int force_plot_data[MAX_FORCEPLOTDATA_BYTES/2]; /* CSAFE_PM_GET_FORCEPLOTDATA */
    unsigned int short heart_beat_data[MAX_HEARTBEATDATA_BYTES/2];  /* CSAFE_PM_GET_HEARTBEATDATA */
    unsigned char workout_state;        /* WORKOUT_STATE; CSAFE_PM_GET_WORKOUTSTATE */
    unsigned int  rest_time;            /* CSAFE_PM_GET_RESTTIME */
    unsigned char interval_type;        /* CSAFE_PM_GET_INTERVALTYPE */
    unsigned char workout_interval_count;   /* CSAFE_PM_GET_WORKOUTINTERVALCOUNT */
} csafe_data_t;

void csafe_init_data(csafe_data_t *o);
int csafe_init_standard_cmd(csafe_buffer_t *b);
int csafe_init_extended_cmd(csafe_buffer_t *b);
int csafe_push_cmd(csafe_cmd c, csafe_buffer_t *b, csafe_data_t *o);
int csafe_end_cmd(csafe_buffer_t *b);
int csafe_unpack(csafe_buffer_t *b, csafe_data_t *o);

#endif /* CSAFE_H */
