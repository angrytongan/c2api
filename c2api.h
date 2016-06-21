#ifndef C2API_H
#define C2API_H

#include "c2api_priv.h"
#include "csafe.h"

#define C2ERR_NONE                  0
#define C2ERR_UNIMPLEMENTED         1
#define C2ERR_FAILED_HID_INIT       2
#define C2ERR_FAILED_ALLOC_ROWERS   3
#define C2ERR_FAILED_GETTIMEOFDAY   4
#define C2ERR_FAILED_LOCALTIME      5

void c2init(c2ctx_t *ctx);
int c2discover(c2ctx_t *ctx);
void c2close(c2ctx_t *ctx);
int c2error(c2ctx_t *ctx);

int c2syncClock(c2ctx_t *ctx, int id);

int c2reset(c2ctx_t *c, int id);
int c2setWorkoutTime(c2ctx_t *c, int id, int h, int m, int s);
int c2setWorkoutDistance(c2ctx_t *c, int id, int distance);
int c2setWorkoutCalories(c2ctx_t *c, int id, int calories);
int c2setProgram(c2ctx_t *c, int id, int prognum);
int c2readyMonitor(c2ctx_t *c, int id);

int c2get(c2ctx_t *c, int id, char *cmd, csafe_data_t *d);

/* testing, supplemental */
int c2setState(c2ctx_t *c, int id, char *cmd);

#endif /* C2API_H */
