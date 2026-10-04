#include <stdio.h>
#include <fcntl.h>     // open(), O_RDONLY
#include <unistd.h>    // read(), close()
#include <errno.h>     // errno
#include <string.h>    // strerror()

int main(void) {
    /* 1. open the same file twice -> watch the fd numbers */
    int fd1 = open("hello.txt", O_RDONLY);
    int fd2 = open("hello.txt", O_RDONLY);
    printf("fd1 = %d, fd2 = %d   (small integers, handed out in order)\n", fd1, fd2);

    /* 2. read from fd1 */
    char buf[128];
    ssize_t n = read(fd1, buf, sizeof(buf) - 1);   /* ssize_t = signed: can be -1 */
    printf("read(fd1, ...) returned %zd bytes\n", n);
    if (n > 0) { buf[n] = '\0'; printf("contents: %s", buf); }

    /* 3. close both */
    close(fd1);
    close(fd2);

    /* 4. a file that does not exist -> errno */
    int fd3 = open("nope.txt", O_RDONLY);
    if (fd3 == -1) {
        int e = errno;                 /* capture IMMEDIATELY */
        perror("open nope.txt");       /* prints: <msg>: <errno message> */
        printf("  errno = %d (%s)\n", e, strerror(e));
    }

    /* 5. the input device -> permission denied (until you re-login) */
    int fdk = open("/dev/input/event4", O_RDONLY);
    if (fdk == -1) {
        int e = errno;
        perror("open /dev/input/event4");
        printf("  errno = %d (%s)\n", e, strerror(e));
    } else {
        printf("opened /dev/input/event4 -> fd = %d (you have access!)\n", fdk);
        close(fdk);
    }
    return 0;
}
