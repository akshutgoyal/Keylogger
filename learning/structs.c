#include <stdio.h>
#include <stdint.h>

typedef enum { 
    RELEASE = 0, 
    PRESS = 1, 
    REPEAT = 2 
} key_state;

typedef struct {
    uint16_t type;
    uint16_t code;
    int32_t  value;
} key_event;

void print_event(const key_event *e) {
    printf("type=%u code=%u value=%d\n", e->type, e->code, e->value);
}

int main(void) {
    key_event e = { PRESS, 30, PRESS };   // pretend: KEY_A pressed
    print_event(&e);
    return 0;
}