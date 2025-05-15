// Write rightrot(x,n) that returns the value of integer x rotated to the right by n positions
#include <stdio.h>
#include <limits.h>

unsigned rightrot(unsigned x, int n) {
    unsigned word_size = sizeof(x) * CHAR_BIT; // sizeof() returns BYTE NUMBER
    n = n%word_size;
    // 2nd term is to move the dropped bits to the beginning
    return (x>>n) | (x<<(word_size-n));
}

int main() {
    unsigned x = 0x0000000f;
    int n = 3;
    printf("0x%X\n", rightrot(x, n)); // should be 0xE0000001
    return 0;
}