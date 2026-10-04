#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <linux/input.h>

/* ---- modifier bits (Lesson 2's bitmask) ---- */
enum {
    MOD_SHIFT = 1 << 0,
    MOD_CTRL  = 1 << 1,
    MOD_ALT   = 1 << 2,
    MOD_CAPS  = 1 << 3
};
static unsigned mods = 0;

/* ---- layout tables, indexed by keycode ---- */
static char base[KEY_MAX + 1];      /* unshifted char, 0 = none */
static char shifted[KEY_MAX + 1];   /* shifted char,   0 = none */

static void setkey(unsigned code, char lo, char hi) {
    base[code]    = lo;
    shifted[code] = hi;
}

static void init_tables(void) {
    /* letters: a-z / A-Z */
    setkey(KEY_A, 'a', 'A'); setkey(KEY_B, 'b', 'B'); setkey(KEY_C, 'c', 'C');
    setkey(KEY_D, 'd', 'D'); setkey(KEY_E, 'e', 'E'); setkey(KEY_F, 'f', 'F');
    setkey(KEY_G, 'g', 'G'); setkey(KEY_H, 'h', 'H'); setkey(KEY_I, 'i', 'I');
    setkey(KEY_J, 'j', 'J'); setkey(KEY_K, 'k', 'K'); setkey(KEY_L, 'l', 'L');
    setkey(KEY_M, 'm', 'M'); setkey(KEY_N, 'n', 'N'); setkey(KEY_O, 'o', 'O');
    setkey(KEY_P, 'p', 'P'); setkey(KEY_Q, 'q', 'Q'); setkey(KEY_R, 'r', 'R');
    setkey(KEY_S, 's', 'S'); setkey(KEY_T, 't', 'T'); setkey(KEY_U, 'u', 'U');
    setkey(KEY_V, 'v', 'V'); setkey(KEY_W, 'w', 'W'); setkey(KEY_X, 'x', 'X');
    setkey(KEY_Y, 'y', 'Y'); setkey(KEY_Z, 'z', 'Z');
    /* digits and their shifted symbols */
    setkey(KEY_1, '1', '!'); setkey(KEY_2, '2', '@'); setkey(KEY_3, '3', '#');
    setkey(KEY_4, '4', '$'); setkey(KEY_5, '5', '%'); setkey(KEY_6, '6', '^');
    setkey(KEY_7, '7', '&'); setkey(KEY_8, '8', '*'); setkey(KEY_9, '9', '(');
    setkey(KEY_0, '0', ')');
    /* punctuation */
    setkey(KEY_MINUS, '-', '_');       setkey(KEY_EQUAL, '=', '+');
    setkey(KEY_LEFTBRACE, '[', '{');   setkey(KEY_RIGHTBRACE, ']', '}');
    setkey(KEY_SEMICOLON, ';', ':');   setkey(KEY_APOSTROPHE, '\'', '"');
    setkey(KEY_GRAVE, '`', '~');       setkey(KEY_BACKSLASH, '\\', '|');
    setkey(KEY_COMMA, ',', '<');       setkey(KEY_DOT, '.', '>');
    setkey(KEY_SLASH, '/', '?');
}

/* ---- keep modifier state in sync with press/release events ---- */
static void update_mods(unsigned code, int value) {
    if (value != 0 && value != 1) return;    /* ignore autorepeat (value 2) */
    int down = (value == 1);

    switch (code) {
        case KEY_LEFTSHIFT:
        case KEY_RIGHTSHIFT: if (down) mods |= MOD_SHIFT; else mods &= ~MOD_SHIFT; break;
        case KEY_LEFTCTRL:
        case KEY_RIGHTCTRL:  if (down) mods |= MOD_CTRL;  else mods &= ~MOD_CTRL;  break;
        case KEY_LEFTALT:
        case KEY_RIGHTALT:   if (down) mods |= MOD_ALT;   else mods &= ~MOD_ALT;   break;
        case KEY_CAPSLOCK:   if (value == 1) mods ^= MOD_CAPS; break;  /* toggle */
        default: break;
    }
}

/* ---- keycode -> printable text or [TOKEN] ---- */
static void token_for(unsigned code, char *out, size_t n) {
    switch (code) {
        case KEY_SPACE:      snprintf(out, n, "[SPACE]");     return;
        case KEY_ENTER:      snprintf(out, n, "[ENTER]");     return;
        case KEY_BACKSPACE:  snprintf(out, n, "[BACKSPACE]"); return;
        case KEY_TAB:        snprintf(out, n, "[TAB]");       return;
        case KEY_ESC:        snprintf(out, n, "[ESC]");       return;
        case KEY_LEFTSHIFT:
        case KEY_RIGHTSHIFT: snprintf(out, n, "[SHIFT]");     return;
        case KEY_LEFTCTRL:
        case KEY_RIGHTCTRL:  snprintf(out, n, "[CTRL]");      return;
        case KEY_LEFTALT:
        case KEY_RIGHTALT:   snprintf(out, n, "[ALT]");       return;
        case KEY_CAPSLOCK:   snprintf(out, n, "[CAPSLOCK]");  return;
        case KEY_UP:         snprintf(out, n, "[UP]");        return;
        case KEY_DOWN:       snprintf(out, n, "[DOWN]");      return;
        case KEY_LEFT:       snprintf(out, n, "[LEFT]");      return;
        case KEY_RIGHT:      snprintf(out, n, "[RIGHT]");     return;
        case KEY_HOME:       snprintf(out, n, "[HOME]");      return;
        case KEY_END:        snprintf(out, n, "[END]");       return;
        case KEY_PAGEUP:     snprintf(out, n, "[PAGEUP]");    return;
        case KEY_PAGEDOWN:   snprintf(out, n, "[PAGEDOWN]");  return;
        case KEY_INSERT:     snprintf(out, n, "[INSERT]");    return;
        case KEY_DELETE:     snprintf(out, n, "[DELETE]");    return;
        default: break;
    }

    char c = base[code];
    if (c == 0) { snprintf(out, n, "[UNKNOWN]"); return; }

    if (c >= 'a' && c <= 'z') {
        /* letters: uppercase if SHIFT and CAPS differ (XOR) */
        int upper = ((mods & MOD_SHIFT) != 0) ^ ((mods & MOD_CAPS) != 0);
        if (upper) c = shifted[code];
    } else if (mods & MOD_SHIFT) {
        /* symbols/digits: shift picks the alternate character */
        if (shifted[code]) c = shifted[code];
    }
    snprintf(out, n, "%c", c);
}

int main(void) {
    init_tables();

    int fd = open("/dev/input/event4", O_RDONLY);
    if (fd == -1) { perror("open /dev/input/event4"); return 1; }

    printf("type away (Ctrl+C to stop)\n");
    fflush(stdout);

    struct input_event ev;
    for (;;) {
        ssize_t n = read(fd, &ev, sizeof ev);
        if (n == -1) {
            if (errno == EINTR) continue;
            perror("read");
            break;
        }
        if (n != (ssize_t)sizeof ev) break;
        if (ev.type != EV_KEY) continue;

        update_mods(ev.code, ev.value);

        if (ev.value == 1) {                 /* key press */
            char tok[32];
            token_for(ev.code, tok, sizeof tok);
            printf("%s", tok);
            fflush(stdout);
        }
    }
    close(fd);
    return 0;
}
