// Our 'getch' and 'ungetch' do not handle EOF properly. Fix this
// Answer: change buffer from 'char' to 'int' as EOF has value -1
#include <stdio.h>

#define BUFSIZE 100

int buf[BUFSIZE];
int bufp = 0;

int getch() {
	return (bufp>0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
	if (bufp > BUFSIZE) {
		printf("ungetch: too many characters\n");
	} else {
		buf[bufp++] = c;
	}
}

int main() {
	int c = '*';
	while((c=getch())!=EOF) {
		putchar(c);
	}
	return 0;
}

