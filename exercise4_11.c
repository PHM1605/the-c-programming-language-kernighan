// Modify 'getop' so that it doesn't need to use ungetch. Hint: use an internal 'static' variable
// Solution: use static variable 'lastc'
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP 100
#define NUMBER '0'

// Stack
#define MAXVAL 100
int sp = 0;
double val[MAXVAL];

// Buffer 
#define BUFSIZE 100
char buf[BUFSIZE];
int bufp;

void push(double f) {
    if (sp<MAXVAL)
        val[sp++] = f;
    else 
        printf("error: stack full, can't push %g\n", f);
}

double pop(void) {
    if (sp>0)
        return val[--sp];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

int getch() {
    return (bufp>0) ? buf[--bufp] : getchar();
}

int getop(char s[]) {
    int i, c;
    // NOTE: lastc, as it's 'static', keeps its value when call 'getop()' multiple times
    static int lastc = 0;
    if (lastc == 0)
        c = getch();
    else {
        c = lastc;
        lastc = 0;
    }

    while((s[0] = c) == ' ' || c=='\t')
        c = getch();
    s[1] = '\0';

    if (!isdigit(c) && c!='.')
        return c;

    i = 0;
    if(isdigit(c))
        while(isdigit(s[++i] = c = getch()));
    if (c == '.')
        while(isdigit(s[++i] = c = getch()));
    s[i] = '\0';

    if (c!=EOF) 
        lastc = c;

    return NUMBER;
}

int main(void) {
    int type;
    double op2;
    char s[MAXOP];

    while((type = getop(s)) != EOF) {
        switch(type){
        case NUMBER:
            push(atof(s));
            break;    
        
        case '+':
            push(pop() + pop());
            break;
        
        case '*':
            push(pop() * pop());
            break;
        
        case '-':
            op2 = pop();
            push(pop() - op2);
            break;
        
        case '/':
            op2 = pop();
            if (op2 != 0.0) 
                push(pop() / op2);
            break;

        case '\n':
            printf("\t%.8g\n", pop());
            break;
        }
    }
    return 0;
}