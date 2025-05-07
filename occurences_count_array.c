// Using array to count occurences of each digit, white space, and others
#include <stdio.h>

/* 
// Using else-if 
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
*/

// Using while loop 
int main() {
	int c, i, nwhite, nother, ndigit[10];
	// Initialization
	nwhite = nother = 0;
	for(i=0; i<10; i++) {
		ndigit[i] = 0;
	}
	while((c=getchar()) != EOF) {
		switch(c) {
			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
				ndigit[c-'0']++;
				break;
			case ' ':
			case '\n':
			case '\t':
				nwhite++;
				break;
			default:
				nother++;
				break;
		}
	}
	printf("digits=");
	for (i=0; i<10; i++)
		printf(" %d", ndigit[i]);
	printf(", white space = %d, other=%d\n", nwhite, nother);
	return 0;
}