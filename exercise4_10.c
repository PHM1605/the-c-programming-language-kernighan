// For the calculator app, use getline to get an entire input line
// Usage: 3 4 +\n => 7
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXLINE 1000
#define MAXOP 1000
#define MAXVAL 1000
#define NUMBER '0'

char line[MAXLINE];
int li = 0; // input line index

// Stack implementation
double val[MAXVAL];
int sp=0; // stack position

int mgetline(char s[], int lim) {
	int i, c;
	for(i=0; i<lim-1 && (c=getchar())!=EOF && c!='\n'; ++i)
		s[i] = c;
	if (c=='\n')
		s[i++] = c;
	s[i] = '\0';
	return i;
}

int getop(char s[]) {
	int i= 0;
	while(line[li]==' ' || line[li]=='\t')
		li++;
	if (line[li] == '\0')
		return '\0';
	s[i]= line[li];
	s[1] = '\0';
	if (!isdigit(line[li]) && line[li]!='.' && line[li]!='-')
		return line[li++];
	
	if (line[li] == '-') {
		if(isdigit(line[li+1]) || line[li+1]=='.') {
			s[i++] = line[li++];
		} else {
			return line[li++];
		}
	}
	// get the integer part 
	if (isdigit(line[li])) {
		while(isdigit(line[li])) {
			s[i++] = line[li++];
		}
	}
	// get the floating part after '.'
	if (line[li] == '.') {
		while(isdigit(line[li]))
			s[i++] = line[li++];
	}
	s[i] = '\0';
	return NUMBER;
}

void push(double f) {
	if (sp<MAXVAL) {
		val[sp++] = f;
	} else {
		printf("error: stack full\n");
	}
}

double pop(void) {
	if (sp>0) 
		return val[--sp];
	else {
		printf("error: stack empty\n");
	}
}

int main() {
	int type;
	char s[MAXOP];
	double op2;
	
	while(mgetline(line, MAXLINE) > 0) {
		li = 0;
		while ((type = getop(s)) != '\0') {
			switch(type) {
				case NUMBER:
					push(atof(s));
					break;
				case '+':
					push(pop() + pop());
					break;
				case '-':
					op2 = pop();
					push(pop() - op2);
					break;
				case '*':
					push(pop() * pop());
					break;
				case '/':
					op2 = pop();
					if (op2 != 0) 
						push(pop() / op2);
					else 
						printf("error: zero divisor\n");
					break;
				case '\n':
					printf("\t%.8g\n", pop());
					break;
				default:
					printf("error: unknown command %s\n", s);
					break;
			}
		}
	}

	return 0;
}
