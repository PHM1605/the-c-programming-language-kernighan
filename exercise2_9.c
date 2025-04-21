// Notice: x &= (x-1) deletes the right-most '1' bit of x
// Use this to write a faster version of bitcount (count the number of '1' bits)

/*
00 -> 11 -> 00
01 -> 00 -> 00
10 -> 01 -> 00
11 -> 10 -> 10
*/
#include <stdio.h>

int bitcount(unsigned x) {
    int count = 0;
    while(x>0) {
        count ++;
        x &= (x-1); 
    }
    return count;
}

int main() {
    unsigned x = 0x31;
    printf("Number of 1 bits in 0x31 is: %d", bitcount(x)); // should be 3
    return 0;
}