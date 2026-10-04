#define _POSIX_C_SOURCE 200809L   /* expose POSIX sigaction; MUST precede ANY #include */

#include <stdio.h>
#include <signal.h>     /* sigaction, SIGINT */
#include <unistd.h>     /* pause, getpid */
#include <errno.h>      /* errno, EINTR */
#include <string.h>     /* strerror, memset */

static volatile sig_atomic_t stop = 0;   /* the ONLY safe kind of shared flag */

static void on_signal(int sig) {
    (void)sig;
    stop = 1;                 /* handler does NOTHING else */
}

int main(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;          /* no SA_RESTART -> blocking calls return EINTR */
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    printf("pid %d: blocking. Send SIGINT (Ctrl+C) to stop.\n", (int)getpid());
    fflush(stdout);           /* flush BEFORE blocking, or you see nothing */

    int wakes = 0;
    while (!stop) {
        errno = 0;
        pause();              /* stands in for the blocking read() you'll use */
        wakes++;
        printf("  pause() returned: errno=%d (%s), stop=%d\n",
               errno, strerror(errno), (int)stop);
    }

    printf("clean shutdown after %d wake(s)\n", wakes);
    return 0;
}
