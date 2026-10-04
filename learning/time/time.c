#define _POSIX_C_SOURCE 200809L   /* POSIX clock_gettime/CLOCK_MONOTONIC; MUST precede ANY #include */

#include <stdio.h>
#include <time.h>

double now_seconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(void) {
    
    char buf[64];
    time_t now = time(NULL);
    struct tm *lt = localtime(&now);
    strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", lt);
    printf("wall clock (REALTIME): %s\n", buf);

    double t0 = now_seconds();
    long sum = 0;
    for (long i = 0; i < 10000000; i++) sum += i;
    double t1 = now_seconds();
    printf("Sum = %ld\n", sum);
    printf("busy loop elapsed: %.3f s\n", t1 - t0);

    now = time(NULL);
    lt = localtime(&now);
    strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", lt);
    printf("wall clock (REALTIME): %s\n", buf);

    return 0;
}