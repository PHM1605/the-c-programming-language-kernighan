// Write setbits(x,p,n,y) that set n bits of x at position p to the rightmost n bits of y
#include <stdio.h>

unsigned setbits(unsigned x, int p, int n, unsigned y) {
    return (x>>(p-n+1) & ~(~0 << n)) | (y & (~0 << n));
} 

int main() {
    unsigned y = 0xff;
    unsigned x = 0x30;
    // '#' means printing '0x' prior; '04' means padding with '0' and total field width is '4'
    printf("%#04x", setbits(x, 7, 4, y)); // should be 0xf3
    return 0;
}