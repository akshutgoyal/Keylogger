#include "keys.h"

#define KEYMAP_SIZE 768
static char keymap[KEYMAP_SIZE];

void keys_init(void) {
    keymap[30] = 'a';
    keymap[31] = 's';
    keymap[32] = 'd';
    keymap[33] = 'f';
    keymap[34] = 'g';
    keymap[57] = ' ';
    keymap[28] = '\n';
    keymap[14] = '\b';
    keymap[15] = '\t';
    keymap[1]  = 27;
}

char key_translate(int code, unsigned mods) {
    if (code < 0 || code >= KEYMAP_SIZE) return 0;
    char ch = keymap[code];
    if (ch >= 'a' && ch <= 'z' && (mods & MOD_SHIFT)) ch -= 32;
    return ch;
}