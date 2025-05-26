// write a Reverse Polish Calculator using scanf
// usage: 3 4 + => Ctrl+D to see result
#include <stdio.h>
#include <stdlib.h> // for atof
#include <ctype.h>
#include <string.h>

#define MAXOP 100 // max size of operand or operator
#define NUMBER '0' // signal that a number was found 
#define MAXVAL 100 // maximum depth of val stack

// stack
int sp = 0;
double val[MAXVAL];

void push(double f) {
    if (sp < MAXVAL) 
        val[sp++] = f;
    else 
        printf("error: stack full, can't push%g\n", f);
}

double pop(void) {
    if (sp>0)
        return val[--sp];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

// main part of change: using scanf
int getop(char* s) {
    if (scanf("%s", s) == EOF)
        return EOF;
    // check if it's a number
    char* endptr;
    // modify and put endptr to the end of the string that needs to be converted
    // to make type-safe for e.g. "ab123" or "123ab" -> wrong
    double d = strtod(s, &endptr); 
    if (endptr != s && *endptr == '\0') 
        return NUMBER;
    return s[0]; // return operator '+','-',...
}

int main() {
    char s[MAXOP]; // store operand/operator
    int type;
    double op2;

    printf("Enter RPN expressions (e.g.`3 4 +`):\n");
    while((type = getop(s))!=EOF) {
        switch(type) {
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
                    push(pop()/op2);
                else
                    printf("error: zero divisor\n");
                break;
            default:
                printf("error: unknown command %s\n", s);
                break;
        }
    }
    // print final result
    if (sp > 0)
        printf("Result: %.8g\n", pop());
    return 0;
}