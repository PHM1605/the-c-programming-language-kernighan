// Suppose that there will be never more than one character of pushback. Modify getch and ungetch accordingly
#include <stdio.h>

char buf = 0;

int getch() {
	int ret;
	// return 'buf' if available, else getchar()
	if (buf!=0)
		ret = buf;
	else
		ret = getchar();
	buf = 0;
	return ret;
}

void ungetch(int c) {
	if (buf != 0)
		printf("ungetch: too many characters\n");
	else
		buf = c;
}

int main() {
	int c = '*';
	ungetch(c);
	while((c=getch()) != EOF) {
		putchar(c);
	}
	return 0;
}
