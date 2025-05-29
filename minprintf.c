// minimal printf with variable argument list
#include <stdio.h>
#include <stdarg.h>

void minprintf(char* fmt, ...) {
    va_list ap; // points to each unnamed arg in turn
    char* p;
    va_start(ap, fmt); // make ap point to 1st unnamed arg
    // to hold argument values
    char* sval;
    int ival;
    double dval;

    for (p=fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }
        p++;
        switch(*p) {
        case 'd':
            ival = va_arg(ap, int); // process ap and move to the next argument
            printf("%d", ival);
            break;
        case 'f':
            dval = va_arg(ap, double);
            printf("%f", dval);
            break;
        case 's':
            sval = va_arg(ap, char*);
            while(*sval) 
                putchar(*sval++);
            break;
        case '%':
            putchar('%'); // handle %%
            break;
        default:
            putchar('%');
            putchar(*p);
            break;
    
        }
    }
    va_end(ap); // clean-up ap when done
}

int main() {
    char* s = "World";
    minprintf("Hello %s\n", s);
    return 0;
}