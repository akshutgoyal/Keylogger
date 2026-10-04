#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <linux/input.h>

#define KEY_COUNT (KEY_MAX + 1)

static unsigned long counts[KEY_COUNT];      /* frequency table */
static const char   *names[KEY_COUNT];       /* display name per code */

static void name_key(unsigned code, const char *n) { names[code] = n; }

static void report(unsigned long total, unsigned long backspaces, double seconds) {
    double minutes = seconds / 60.0;
    double wpm = (total / 5.0) / (minutes > 0 ? minutes : 1e-9);

    printf("=== session report ===\n");
    printf("total keystrokes : %lu\n", total);
    printf("runtime          : %.1f s\n", seconds);
    printf("backspaces       : %lu (%.1f%%)\n",
           backspaces, 100.0 * backspaces / (total ? total : 1));
    printf("avg WPM          : %.1f\n", wpm);

    /* find the biggest count so bars scale to it */
    unsigned long max = 0;
    for (int i = 0; i < KEY_COUNT; i++)
        if (counts[i] > max) max = counts[i];

    printf("\ntop 5 keys:\n");
    for (int rank = 0; rank < 5; rank++) {
        int best = -1;
        unsigned long bestc = 0;
        /* repeatedly pick the current maximum... */
        for (int i = 0; i < KEY_COUNT; i++)
            if (counts[i] > bestc) { bestc = counts[i]; best = i; }
        if (best < 0) break;
        /* ...then zero it so the next round finds the next one */
        int bar = max ? (int)(40.0 * bestc / max) : 0;
        printf("  %-10s %6lu  ", names[best] ? names[best] : "?", bestc);
        for (int b = 0; b < bar; b++) putchar('#');
        putchar('\n');
        counts[best] = 0;
    }
}

int main(void) {
    /* fabricate a small session so the formatting is visible */
    name_key(KEY_E, "E");       name_key(KEY_SPACE, "SPACE");
    name_key(KEY_T, "T");       name_key(KEY_A, "A");
    name_key(KEY_O, "O");       name_key(KEY_N, "N");
    name_key(KEY_S, "S");       name_key(KEY_BACKSPACE, "BACKSPACE");

    counts[KEY_E] = 50;   counts[KEY_SPACE] = 48;   counts[KEY_T] = 30;
    counts[KEY_A] = 28;   counts[KEY_O] = 22;       counts[KEY_N] = 18;
    counts[KEY_S] = 15;   counts[KEY_BACKSPACE] = 8;

    unsigned long total = 0;
    for (int i = 0; i < KEY_COUNT; i++) total += counts[i];

    report(total, counts[KEY_BACKSPACE], 132.0);   /* pretend 2.2 minutes */
    return 0;
}
