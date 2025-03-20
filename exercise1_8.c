/** Count blanks, tabs and new lines
*/

#include <stdio.h>

int main() {
	int n_blanks=0, n_tabs=0, n_lines=0;
	int c;
	while((c=getchar())!=EOF) {
		if (c==' ') {
			n_blanks++;
		} else if (c=='\t') {
			n_tabs++;
		} else if (c=='\n') {
			n_lines++;
		}
	}
	printf("Number of blanks: %d\n", n_blanks);
	printf("Number of tabs: %d\n", n_tabs);
	printf("Number of lines: %d\n", n_lines);
	return 0;
}
