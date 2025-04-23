/* Polish calculator (1-2)*(4+5) => 1 2 - 4 5 + *
using a stack:
while (next operator or operand is not EOF)
    if (number)
        push-to-stack
    else if (operator)
        pop 1 or 2 operands (depending on operator)
        do operation
        push result
    else if (newline)
        pop and print top of stack
    else
        error
*/
// try by typing: 1 2 - 4 5 + * => should be -9
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP 100   // max length of string containing operand/operator
#define NUMBER '0' // signal that a number was found 
#define MAXVAL 100 // stack maximum #elements
#define BUFSIZE 100 // to 'get' and 'unget' a character

double val[MAXVAL]; // stack
int sp = 0; // next free stack position

char buf[BUFSIZE]; // buffer
int bufp = 0; // next free buffer position

int getch(void) {
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

// get op from Polish string
int getop(char s[]) {
    int i; // to fill the resulting string
    int c; // get the next char, either from buffer or getchar()
    while((s[0] = c = getch()) == ' ' || c=='\t'); // consuming all spaces, to ensure we are getting a number, an operand or '\n' to end
    s[1] = '\0';
    if (!isdigit(c) && c!='.')
        return c; // not a number
    i = 0;
    if (isdigit(c))
        while (isdigit(s[++i] = c = getch()));
    if (c=='.')
        while (isdigit(s[++i] = c = getch()));
    s[i] = '\0';
    if (c!=EOF)
        ungetch(c);
    return NUMBER;
} 

// push float to stack
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

int main() {
    int type; // operator or operand or EOF
    double op2; // what being pulled from the stack
    char s[MAXOP]; 

    while((type = getop(s)) != EOF) {
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
                if (op2 != 0.0)
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
    return 0;
}