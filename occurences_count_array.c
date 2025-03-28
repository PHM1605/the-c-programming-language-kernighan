// Using array to count occurences of each digit, white space, and others
#include <stdio.h>

int main() {
	int c, nwhite, nother;
	int ndigit[10];
	// Initialization
	nwhite = nother = 0;
	for (int i=0; i<10; ++i) {
		ndigit[i] = 0;
	}
	// Get chars loop
	while ((c=getchar()) != EOF) {
		if (c>='0' && c<='9')
			++ndigit[c-'0'];
		else if (c==' ' || c=='\n' || c=='\t')
			++nwhite;
		else
			++nother;
	}
	// Result
	printf("digits = ");
	for (int i = 0; i<10; ++i)
		printf(" %d", ndigit[i]);
	printf(", white space = %d, other = %d\n", nwhite, nother);

	return 0;
}
