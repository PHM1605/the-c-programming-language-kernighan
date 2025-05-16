// For the calculator app, use getline to get an entire input line
#include <stdio.h>

#define MAXLINE 1000

char line[MAXLINE];
int li = 0; // input line index

int mgetline(char s[]) {
	int i, c;
	for(i=0; i<MAXLINE-1 && (c=getchar())!=EOF && c!='\n'; ++i)
		s[i] = c;
	if (c=='\n')
		s[i++] = c;
	s[i] = '\0';
	return i;
}

int getop(char s[]) {
	int c, i;
	if (line[li]=='\0') {

	}

int main() {
	int type;
	
	while((type = getop(s)) != EOF) {
		switch(type) {
			case NUMBER:
				push(atof(s));
				break;

	}

	return 0;
}
