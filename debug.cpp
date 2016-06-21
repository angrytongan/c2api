#ifdef DEBUG
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#define FG_RED      "\x1b[31m"
#define FG_YELLOW   "\x1b[33m"
#define FG_NORMAL   "\x1b[0m"

#define DEBUG_NORMAL    0
#define DEBUG_WARNING   1
#define DEBUG_ERROR     2

static void _debug(int type, const char *line) {
    bool is_a_tty = isatty(1);

    if (type == DEBUG_NORMAL) {
        fprintf(stderr, "%s\n", line);
    } else if (type == DEBUG_WARNING) {
        if (is_a_tty) {
            fprintf(stderr, FG_YELLOW"%s"FG_NORMAL"\n", line);
        } else {
            fprintf(stderr, "%s\n", line);
        }
    } else {
        if (is_a_tty) {
            fprintf(stderr, FG_RED"%s"FG_NORMAL"\n", line);
        } else {
            fprintf(stderr, "%s\n", line);
        }
    }
}

void debug(const char *fmt, ...) {
    va_list ap;
    char line[128];

    va_start(ap, fmt);
    vsnprintf(line, sizeof(line), fmt, ap);
    va_end(ap);
    _debug(DEBUG_NORMAL, line);
}
void warning(const char *fmt, ...) {
    va_list ap;
    char line[128];

    va_start(ap, fmt);
    vsnprintf(line, sizeof(line), fmt, ap);
    va_end(ap);
    _debug(DEBUG_WARNING, line);
}
void error(const char *fmt, ...) {
    va_list ap;
    char line[128];

    va_start(ap, fmt);
    vsnprintf(line, sizeof(line), fmt, ap);
    va_end(ap);
    _debug(DEBUG_ERROR, line);
}
#else   /* DEBUG */
void error(const char *fmt, ...) {
    return;
}
void warning(const char *fmt, ...) {
    return;
}
void debug(const char *fmt, ...) {
    return;
}
#endif  /* DEBUG */
