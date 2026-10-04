#include <stdio.h>

enum { 
    MOD_SHIFT = 1 << 0, 
    MOD_CTRL = 1 << 1 
};

int main(void) {
    char keymap[768] = {0};
    keymap[30] = 'a';      // KEY_A
    keymap[31] = 's';      // KEY_S
    keymap[57] = ' ';      // KEY_SPACE

    unsigned mods = 0;
    int code = 30;         // pretend KEY_A event

    mods |= MOD_SHIFT;     // pretend shift was held

    char ch = keymap[code];
    if (ch == 0) {
        printf("special key (code %d)\n", code);
    } else {
        if (mods & MOD_SHIFT) ch -= 32;   // 'a' -> 'A' in ASCII
        printf("char: %c\n", ch);
    }
    return 0;
}