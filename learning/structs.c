#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    unsigned long total_keys;
    unsigned long backspaces;
    long start_time;
} session_stats;

void print_stats(const session_stats *s){
    printf("Total keys: %lu\nBackspaces: %lu\nStart time: %ld\n", s->total_keys, s->backspaces, s->start_time);
}

int main(){
    session_stats s = { 100, 5, 1620000000 };
    print_stats(&s);
    return 0;
}