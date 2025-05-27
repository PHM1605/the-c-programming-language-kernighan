// Write 'ungets' that will push back an entire string onto the input.
// Usage: input: "this is a sentence" -> output: "this is a sentence"

#include <stdio.h>
#include <string.h>

#define BUFSIZE 100
#define MAXLINE 100

char buf[BUFSIZE];
int bufp = 0;

// Read from input to line
int mgetline(char s[], int lim) {
	int i, c;
	for (i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
		s[i] = c;
	
	if (c=='\n')
		s[i++] = c;
	
	s[i] = '\0';
	return i;
}

// Read from line to buffer (buffer has reversed order)
void ungetch(int c) {
	if (bufp >= BUFSIZE) {
		printf("ungetch: too many characters\n");
	} else {
		buf[bufp++] = c;
	}
}
void ungets(char s[]) {
	int len = strlen(s);
	while(len>0) {
		ungetch(s[--len]);
	}
}

// Read from buffer to output
int getch() {
	return (bufp>0) ? buf[--bufp] : getchar();
}


int main() {
	char line[MAXLINE];
	int len;
	while((len = mgetline(line, MAXLINE)) > 0) {
		ungets(line);
		for(int i=0; i<len; i++) {
			putchar(getch());
		}
	}
	
	return 0;
}

