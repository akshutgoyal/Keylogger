#include <stdio.h>
#include <fcntl.h>      /* open, O_RDONLY */
#include <unistd.h>     /* read, close */
#include <errno.h>      /* errno */
#include <string.h>     /* strerror */
#include <linux/input.h>/* struct input_event, EV_* */

static const char *type_name(unsigned t) {
    switch (t) {
        case EV_SYN: return "EV_SYN";
        case EV_KEY: return "EV_KEY";
        case EV_MSC: return "EV_MSC";
        case EV_LED: return "EV_LED";
        case EV_REP: return "EV_REP";
        default:     return "OTHER";
    }
}

int main(void) {
    int fd = open("/dev/input/event4", O_RDONLY);
    if (fd == -1) {
        perror("open /dev/input/event4");
        return 1;
    }

    printf("reading /dev/input/event4 - press keys (Ctrl+C to stop)\n");
    fflush(stdout);

    struct input_event ev;
    for (;;) {
        ssize_t n = read(fd, &ev, sizeof ev);
        if (n == -1) {
            if (errno == EINTR) continue;   /* interrupted by signal, retry */
            perror("read");
            break;
        }
        if (n != (ssize_t)sizeof ev) {
            fprintf(stderr, "short read: %zd\n", n);
            break;
        }
        if (ev.type == EV_KEY && ev.value == 1) {
            printf("key code %u pressed\n", ev.code);
        }
        // printf("%ld.%06ld  %-7s code=%-3u value=%d\n",
        //         (long)ev.time.tv_sec, (long)ev.time.tv_usec,
        //         type_name(ev.type), ev.code, ev.value);
        // }
    }
    close(fd);
    return 0;
}
