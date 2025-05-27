// Write a function invert(x,p,n) that returns x with the n bits that begin at position p inverted

#include <stdio.h>

unsigned invert(unsigned x, int p, int n) {
	int n_trailing = p-n+1;
	unsigned inverted_bits = ~((x>>n_trailing) | (~0<<n)) << n_trailing;
	printf("%#04x\n", inverted_bits);
	
	unsigned rest_bits = x & ~(~(~0<<n) << n_trailing) ;
	printf("%#04x\n", rest_bits);
	return inverted_bits | rest_bits;
}

int main() {
	unsigned x = 0xD7;
	unsigned y = invert(x, 6, 3); 
	printf("Result: %#04x\n", y); // should be 0xA7

	return 0;
}

