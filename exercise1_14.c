// Print histogram of frequencies of characters
#include <stdio.h>

int main() {
	int c;
	int alphabet_length = 'z'-'a';
	int histogram[alphabet_length];
	for (int i=0; i<alphabet_length; i++) {
		histogram[i] = 0;
	}
	while((c=getchar()) != EOF) {
		if (c>='a' && c<='z') {
			histogram[c-'a']++;
		}
	}
	// display histogram
	for (int i=0; i<alphabet_length+1; i++) {
		printf("%c\t", i+'a');
		for (int j=0; j<histogram[i]; j++) {
			putchar('*');
		}
		putchar('\n');
	}
	return 0;
}
