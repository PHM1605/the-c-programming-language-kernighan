// count the number of chars 
// note: Ctrl+D to type EOF to end 'while' loop
#include <stdio.h>

int main() {
	/* // Method 1: use 'long' and 'while' loop
	long nc = 0;
	while (getchar() != EOF)
		++nc;
	printf("%ld\n", nc);
	*/
	//Method 2: use 'double' and 'for' loop
	double nc;
	for (nc=0; getchar() != EOF; ++nc);
	printf("%.0f\n", nc);

	return 0;
}
