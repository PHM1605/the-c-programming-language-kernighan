/**	Copy input to output; replacing many blanks with 1 blank only
*/

#include <stdio.h>
#include <stdbool.h>

int main() {
	int c;
	bool bIsPreviousBlank = false;
	while((c=getchar())!=EOF) {
		if (!bIsPreviousBlank || c!=' ') {
			putchar(c);
		}
		if (c==' ')
			bIsPreviousBlank = true;
		else
			bIsPreviousBlank = false;
	}
	return 0;
}

