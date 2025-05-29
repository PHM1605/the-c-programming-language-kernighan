// function 'isupper' can be implemented to save space or save time. Explore both options
#include <stdio.h>

static char table[256];

void init_table() {
    for (int i='A'; i<='Z'; i++)
        table[i] = 1;
}

int isupper_big_fast(unsigned char c) {
    return table[c];
}

int isupper_compact_slow(unsigned char c) {
    return c>='A' && c<='Z';
}

int main() {
    int c1 = 'a';
    int c2 = 'A';
    // Fast method
    init_table();
    printf("Isupper fast: %d %d\n", isupper_big_fast(c1), isupper_big_fast(c2));

    // Slower method
    printf("Isupper slow: %d %d\n", isupper_compact_slow(c1), isupper_compact_slow(c2));
}