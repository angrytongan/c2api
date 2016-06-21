#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "csafe.h"
#include "csafe_util.h"
#include "c2api.h"
#include "debug.h"

const char *opts[] = { 
    "-h                 help",
    "-r                 reset state machine",
    "-s                 sync clock",
    "-c <calories>      set target calories to calories",
    "-t <h:m:s>         set workout time to h:m:s",
    "-d <metres>        set horizontal distances to metres",
    "-p <program>       use pre-set program number and ready monitor",
    "-g <...>           get and dump: status|monitor|forceplot|workout|erg",
    "-y <...>           set state: finished|haveid|idle|inuse|ready"
};

static void usage(const char *progname) {
    int i;
    int n = sizeof(opts) / sizeof(char*);

    debug("usage: %s [options]", progname);

    for (i = 0; i < n; i++) {
        debug("%s", opts[i]);
    }
}

int main(int argc, char *argv[]) {
    c2ctx_t ctx, *c;
    int ch;
    int hours, minutes, seconds;
    int horiz_distance;
    int calories;
    csafe_data_t d;

    csafe_init_data(&d);

    c = &ctx;

    if (argc == 1) {
        usage(argv[0]);
        return -1;
    }

    c2init(c);
    if (c2discover(c) == -1) {
        warning("%s: c2discover() failed", argv[0]);
        c2close(c);
        return -1;
    }

    while ((ch = getopt(argc, argv, "hrsc:t:d:p:g:y:")) != -1) {
        switch (ch) {
            case 'h':
                usage(argv[0]);
                return -1;
                break;

            case 't':
                if (sscanf(optarg, "%2d:%2d:%2d", &hours, &minutes, &seconds) == -1) {
                    warning("%s: failed to parse %s as hh:mm:ss", argv[0], optarg);
                } else {
                    c2setWorkoutTime(c, -1, hours, minutes, seconds);
                }
                break;

            case 'd':
                if (sscanf(optarg, "%d", &horiz_distance) == -1) {
                    warning("%s: failed to parse %s as metres", argv[0], optarg);
                } else {
                    c2setWorkoutDistance(c, -1, horiz_distance);
                }
                break;

            case 'c':
                if (sscanf(optarg, "%d", &calories) == -1) {
                    warning("%s: failed to parse %s as calories", argv[0], optarg);
                } else {
                    c2setWorkoutCalories(c, -1, calories);
                }
                break;

            case 'r':
                c2reset(c, -1);
                break;

            case 'p':
                c2setProgram(c, -1, atoi(optarg));
                break;

            case 'g':
                c2get(c, -1, optarg, &d);
                if (!strcmp(optarg, "status")) {
                    dump_status(&d);
                } else if (!strcmp(optarg, "monitor")) {
                    dump_monitor(&d);
                } else if (!strcmp(optarg, "forceplot")) {
                    dump_forceplot(&d);
                } else if (!strcmp(optarg, "workout")) {
                    dump_workout(&d);
                } else if (!strcmp(optarg, "erg")) {
                    dump_erg(&d);
                }
                break;

            case 's':
                c2syncClock(c, -1);
                break;

            case 'y':
                c2setState(c, -1, optarg);
                break;

            case '?':
            default:
                usage(argv[0]);
                return -1;
                break;
        }
    }

    c2close(c);

    return 0;
}
