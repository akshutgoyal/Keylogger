#include <stdio.h>
#include "keys.h"

int main(void) {
    keys_init();

    unsigned mods = MOD_SHIFT;
    int code = 31;

    char ch = key_translate(code, mods);
    char line[64];

    if (ch == 0) {
        snprintf(line, sizeof(line), "special key (code %d)\n", code);
    } else {
        snprintf(line, sizeof(line), "char: %c\n", ch);
    }
    printf("%s", line);
    return 0;
}