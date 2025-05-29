// gcc exercise5_10.c -o expr
// evaluate reverse polish calculator from the command line
// e.g. expr 2 3 4 + \* -> (3+4)*2 = 14
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXSTACK 100

// stack
double stack[MAXSTACK];
int sp = 0; 

void push(double f) {
    if (sp<MAXSTACK) 
        stack[sp++] = f;
    else
        printf("error: stack full, can't push %g\n", f);
}

double pop() {
    if (sp>0)
        return stack[--sp];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}

int is_number(char* s) {
    char* end;
    strtod(s, &end); // will return a double, but we don't need
    return *end == '\0';
}

int main(int argc, char* argv[]) {
    for (int i=1; i<argc; ++i) {
        char* arg = argv[i];
        if (is_number(arg)) {
            push(atof(arg));
        } else if (strlen(arg) == 1) {
            double op2;
            switch(arg[0]) {
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
                if (op2!=0.0)
                    push(pop()/op2);
                else {
                    printf("Error: division by zero\n");
                    return 1;
                }
                break;
            default:
                printf("Error: unknown operator '%s'\n", arg);
                return 1;
            }
        } else {
            printf("Error: invalid token '%s'\n", arg);
            return 1;
        }
    }
    
    // print if correct format
    if (sp != 1) {
        printf("Error: invalid expression (stack size = %d)\n", sp);
        return 1;
    }
    printf("Result: %g\n", pop());


    return 0;
}