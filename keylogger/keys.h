#ifndef KEYS_H
#define KEYS_H

#include <stddef.h>

enum {
    MOD_SHIFT = 1 << 0,
    MOD_CTRL  = 1 << 1,
    MOD_ALT   = 1 << 2,
    MOD_CAPS  = 1 << 3
};

void     keys_init(void);
void     keys_update_mods(unsigned code, int value);
unsigned keys_mods(void);
void     key_token(unsigned code, char *out, size_t n);

#endif
