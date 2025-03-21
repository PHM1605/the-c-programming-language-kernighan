// Print a histogram of lengths of words in it's input (horizontally)
#include <stdio.h>

#define OUT 0 // typing outside a word
#define IN 1

int main() {
	int c;
	int state = OUT;
	int count = 0;
	int lengths[10];
	for (int i=0; i<10; i++) {
		lengths[i]=0;
	}
	while((c=getchar()) != EOF) {
		if (c==' ' || c=='\t' || c== '\n') {
			if (state == IN) {
				lengths[count]++;
				count = 0;
			}
			state = OUT;
		}
		else {
			count++;
			state = IN;
		}
	}
	// display count
	for (int i=1; i<10; i++) {
		printf("%d\t", i);
		for (int j=0; j<lengths[i]; j++) {
			putchar('*');
		}
		putchar('\n');
	}
	return 0;
}
