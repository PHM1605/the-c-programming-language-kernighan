// verify that the value of getchar() != EOF is 0 or 1

#include <stdio.h>

int main() {
	int c = getchar() != EOF;
	printf("Value of getchar()!=EOF is: %d\n", c);
	printf("Value of EOF is: %d\n", EOF);
	return 0;
}
