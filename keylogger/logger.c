#include "logger.h"

#include <stdio.h>
#include <time.h>

static FILE *log = NULL;

int logger_open(const char *path) {
    log = fopen(path, "a");              /* append across sessions */
    return log ? 0 : -1;
}

void logger_write(long sec, long usec, const char *token) {
    if (log == NULL) return;

    time_t t = (time_t)sec;
    struct tm *tm = localtime(&t);          /* epoch -> calendar (local time) */
    char stamp[32];
    if (tm) strftime(stamp, sizeof stamp, "%d/%m/%y %H:%M:%S", tm);
    else    snprintf(stamp, sizeof stamp, "%ld", sec);

    fprintf(log, "[%s.%03ld] %s\n", stamp, usec / 1000, token);
}

void logger_close(void) {
    if (log == NULL) return;
    fflush(log);                          /* push buffered lines to disk */
    fclose(log);
    log = NULL;
}
