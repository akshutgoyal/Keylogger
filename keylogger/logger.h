#ifndef LOGGER_H
#define LOGGER_H

int  logger_open(const char *path);      /* 0 = ok, -1 = failed */
void logger_write(long sec, long usec, const char *token);
void logger_close(void);                 /* flush + close */

#endif
