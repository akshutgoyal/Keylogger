#define _POSIX_C_SOURCE 200809L   /* expose POSIX sigaction/usleep; MUST precede ANY #include */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <signal.h>    
#include <unistd.h>   
#include <errno.h>    
#include <string.h>
#include <time.h> 

static volatile sig_atomic_t stop = 0; 

static void on_signal(int sig) {
    (void)sig;
    stop = 1;
}
int main(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    FILE *fp = fopen("count.log", "w");
    if (fp == NULL) {
        perror("fopen count.log");
        return 1;
    }

    int n = 0;
    struct timespec ts = { 0, 500000000 };   /* 0.5 s */
    while (!stop) {
        fprintf(fp, "line %d\n", n);
        n++;
        nanosleep(&ts, NULL);
    }

    fflush(fp);
    fclose(fp);
    printf("wrote %d lines\n", n);
    return 0;
}