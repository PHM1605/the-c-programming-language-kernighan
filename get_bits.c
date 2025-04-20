// get n bits starting from position p of number x 
unsigned getbits(unsigned x, int p, int n) {
    // x>>(p+1-n) means moving n desired bits to the right
    return (x >> (p+1-n)) & ~(~0 << n);
}