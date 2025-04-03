// Modify the longest line program, printing length of input lines & the text

#include <stdio.h>
#define MAXLINE 1000 // maximum input line length

int get_line(char line[], int maxline);
void copy(char to[], char from[]);

int main() {
	int len; // current line length
	int max = 0; // max length of line seen so far
	char line[MAXLINE]; // to store current line
	char longest[MAXLINE]; // to store longest line so far
	while ((len = get_line(line, MAXLINE)) > 0) {
		printf("Current line: %s has length of %d chars\n", line, len);
		if (len>max) {
			max = len;
			copy(longest, line);
		}
	}
	if (max>0) {
		printf("%s", longest);
	}
	return 0;
}

int get_line(char s[], int lim) {
	int c, i;
	for (i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
		s[i] = c;
	if (c=='\n') {
		s[i] = c;
		++i;
	}
	s[i] = '\0';
	return i;
}

void copy(char to[], char from[]) {
	int i=0;
	while((to[i] = from[i]) != '\0')
		++i;
}
