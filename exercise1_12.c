/** Write a program that prints its input one word per line
*/

#include <stdio.h>

#define IN 1 /* inside a word */
#define OUT 0 /* outside a word */

int main() {
	int c;
	int state = OUT;
	while((c=getchar()) != EOF) {
		if (c==' ' || c=='\t' || c=='\n') {
			if (state==IN) {
				putchar('\n');
			}
			state = OUT;
		} else {
			state = IN;
			putchar(c);
		}
	}
	return 0;
}
