#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <time.h>       /* time, clock_gettime, localtime, strftime, timespec */

int main(void) {
    /* --- 1. WALL CLOCK: "what time is it right now?" --- */
    time_t now = time(NULL);              /* seconds since 1970 */
    struct tm *lt = localtime(&now);      /* break into Y/M/D H:M:S */
    char buf[64];
    strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", lt);
    printf("wall clock (REALTIME): %s\n", buf);

    /* --- 2. MONOTONIC: "how long did this take?" --- */
    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);  /* start marker */

    struct timespec req = { 0, 250000000 };   /* 0.25 s */
    nanosleep(&req, NULL);

    clock_gettime(CLOCK_MONOTONIC, &t1);  /* end marker */
    double elapsed = (t1.tv_sec - t0.tv_sec)
                   + (t1.tv_nsec - t0.tv_nsec) / 1e9;
    printf("monotonic elapsed: %.3f s (expected ~0.250)\n", elapsed);

    /* --- 3. what a raw timestamp looks like --- */
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    printf("raw CLOCK_REALTIME: tv_sec=%ld  tv_nsec=%ld\n",
           (long)ts.tv_sec, (long)ts.tv_nsec);

    return 0;
}
