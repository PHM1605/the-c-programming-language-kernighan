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
// Exercise 4-3: add % operator and support negative numbers e.g. 1 -2 + 6 4 % + => 1
// Exercise 4-4: add the commands to print the stack elements, duplicate stack elements, swap top two elements, clear the stack e.g. 1 2 3 4, then 'la', 'dup', 'swap', 'clear'
// Exercise 4-5: add 'sin', 'exp', 'pow'
/* Exercise 4-6: Add commands for handling variables (provide 26 variables with single-letter names). Add a variable for the most recently printed value. 
5 A = //assign value A=5
3 B = //assign value B=3
A B + //compute A+B
\n //print 8

*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAXOP 100   // max length of string containing operand/operator
#define NUMBER '0' // signal that a number was found 
#define COMMAND '1' // signal that a command was found
#define MAXVAL 100 // stack maximum #elements
#define BUFSIZE 100 // to 'get' and 'unget' a characte

double val[MAXVAL]; // stack
int sp = 0; // next free stack position

char buf[BUFSIZE]; // buffer
int bufp = 0; // next free buffer position

// for exercise 4.6
double variables[16];
double recent = 0.0f;

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
    int ret;
    s[1] = '\0';
    i = 0;
    if (c=='-') {
        if (isdigit(c=getch())) {
            ungetch(c);
            while (isdigit(s[++i] = c = getch()));
            s[i] = '\0';
            return NUMBER;
        } else { // a space => this is an operand
            c = '-'; 
        }            
    }
    if (isalpha(c))  {
        while (isalpha(s[++i] = c = getch()));    
        s[i] = '\0';
        return COMMAND;   
    }
    if (!isdigit(c) && c!='.')
        return c; // not a number

	// reading a number...
    if (isdigit(c))
        while (isdigit(s[++i] = c = getch()));
    // ... or float
		if (c=='.')
        while (isdigit(s[++i] = c = getch()));

    // reading A-Z
    if (isupper(c)) {
        s[0] = c;
        s[1] = '\0';
        return c;
    }

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
            case COMMAND:
                // list all components
                if (strcmp(s, "la")==0) {
                    if (sp==0){
                        printf("Empty stack\n");
                    } else {
                        for (int i =0; i<sp; i++) {
                            printf("%.2g ", val[i]);
                        }
                        putchar('\n');
                    }                    
                } 
                // duplicate what inside stack
                else if (strcmp(s, "dup")==0) {
                    int current_free_loc = sp;
                    for (int i = 0; i<current_free_loc; i++) {
                        push(val[i]);
                    }
                    printf("Done duplicate\n");
                } 
                // swap the last two components
                else if (strcmp(s, "swap") == 0 && sp>=2) {
                    double tmp = pop();
                    double tmp2 = pop();
                    push(tmp);
                    push(tmp2);
                    printf("Done swapping\n");
                }
                // clear all 
                else if (strcmp(s, "clear") == 0) {
                    sp = 0;
                    printf("Done clearing stack\n");
                } 
                else if (strcmp(s, "sin") == 0) {
                        push(sin(pop()));
                }
                else if (strcmp(s, "cos") == 0) {
                        push(cos(pop()));
                }
                else if (strcmp(s, "exp") == 0) {
                        push(exp(pop()));
                } 
                else if (strcmp(s, "pow") == 0) {
                        op2 = pop();
                        push(pow(pop(), op2));
                }
                else if (strcmp(s, "last") == 0) {
                    printf("Last printed: %.8g\n", recent);
                }
                else {
                    printf("error: invalid command\n");
                }
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
            case '%':
                op2 = pop();
                if (op2 <= 0)
                    printf("error: module operator <=0\n");
                else 
                    push((int)pop() % (int)op2);
                break;
            case '\n':
                recent = pop();
                printf("\t%.8g\n", recent);
                break;
            case '=':
                double value = pop(); // variable value e.g. 5
                int varname = pop(); // variable name in terms of char e.g. 'A'
                if (varname >= 'A' && varname <= 'Z') {
                    variables[varname-'A'] = value;
                } else {
                    printf("error: invalid variable name\n");
                }

            default:
                if (type>='A' && type<='Z') {
                    push(variables[type-'A']);
                } else {
                    printf("error: unknown command %s\n", s);
                }                
                break;
        }
    }
    return 0;
}
