#include <stdio.h>
#include <stdlib.h>

enum {
    MOD_SHIFT = 1 << 0,
    MOD_CTRL = 1 << 1
};

int main(){
    char keymap[768] = {0};
    keymap[30] = 'a';      // KEY_A
    keymap[31] = 's';      // KEY_S
    keymap[32] = 'd';      // KEY_D
    keymap[33] = 'f';      // KEY_F
    keymap[34] = 'g';      // KEY_G
    keymap[57] = ' ';      // KEY_SPACE
    keymap[28] = '\n';     // KEY_ENTER
    keymap[14] = '\b';     // KEY_BACKSPACE
    keymap[15] = '\t';     // KEY_TAB
    keymap[1] = 27;       // KEY_ESC
    unsigned mods = 0;

    int code = 30;         // pretend KEY_A event
    
    mods |= MOD_SHIFT;     // pretend shift was held

    char line[64];
    char ch = keymap[code];
    if (ch == 0) {
        snprintf(line, sizeof(line), "special key (code %d)\n", code);
        printf("%s", line);
    } else {
        if (mods & MOD_SHIFT) ch -= 32;   // 'a' -> A' in ASCII
        snprintf(line, sizeof(line), "char: %c\n", ch);
        printf("%s", line);
    }

    return 0;
}