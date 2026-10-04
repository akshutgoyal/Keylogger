#ifndef KEYS_H
    #define KEYS_H

    enum {
        MOD_SHIFT = 1 << 0,
        MOD_CTRL = 1 << 1
    };

    void keys_init(void);
    char key_translate(int code, unsigned mods);

#endif