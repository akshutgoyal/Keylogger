#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <linux/input.h>

#include "keys.h"
#include "logger.h"

static volatile sig_atomic_t stop = 0;
static void on_signal(int sig) { (void)sig; stop = 1; }

/* exit hotkey: hold LEFTCTRL + LEFTALT, then press Q */
static int is_exit_hotkey(unsigned code) {
    unsigned m = keys_mods();
    return code == KEY_Q && (m & MOD_CTRL) && (m & MOD_ALT);
}

int main(void) {
    keys_init();

    if (logger_open("keystrokes.log") != 0) {
        perror("keystrokes.log");
        return 1;
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_handler = on_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT,  &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    int fd = open("/dev/input/event4", O_RDONLY);
    if (fd == -1) {
        perror("/dev/input/event4");
        logger_close();
        return 1;
    }

    printf("recording to keystrokes.log\n");
    printf("exit: hold Ctrl+Alt and press Q  (or Ctrl+C)\n");
    fflush(stdout);

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    unsigned long total = 0;
    struct input_event ev;

    while (!stop) {
        ssize_t n = read(fd, &ev, sizeof ev);
        if (n == -1) {
            if (errno == EINTR) continue;
            perror("read");
            break;
        }
        if (n != (ssize_t)sizeof ev) continue;
        if (ev.type != EV_KEY) continue;

        keys_update_mods(ev.code, ev.value);
        if (ev.value != 1) continue;          /* act on presses only */

        if (is_exit_hotkey(ev.code)) break;   /* don't log the hotkey itself */

        char tok[32];
        key_token(ev.code, tok, sizeof tok);
        logger_write(ev.time.tv_sec, ev.time.tv_usec, tok);
        total++;
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);
    close(fd);
    logger_close();                            /* flush + close on exit */

    double seconds = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;

    printf("\n=== session stats ===\n");
    printf("total keystrokes : %lu\n", total);
    printf("runtime          : %.1f s\n", seconds);
    return 0;
}
