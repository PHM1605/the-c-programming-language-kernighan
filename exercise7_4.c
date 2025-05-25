#include <stdio.h>
#include <stdarg.h>

void minscanf(char* fmt, ...) {
    char* p; // pointer for fmt
    int* ival; // save the address that passed in
    float* fval;
    char* sval;

    va_list ap;
    va_start(ap, fmt);
    for (p = fmt; *p; p++) {
        // skip non-format characters
        if (*p != '%')
            continue;
        p++; // move to the letter next to '%'
        switch(*p) {
        case 'd':
            ival = va_arg(ap, int*); // address (of result) where must be filled
            scanf("%d", ival);
            break;
        case 'f':
            fval = va_arg(ap, float*);
            scanf("%f", fval);
            break;
        case 's':
            sval = va_arg(ap, char*);
            scanf("%s", sval);
            break;
        default:
            break;
        }
    }
    va_end(ap);
}

int main() {
    int i;
    float f;
    char s[100];

    printf("Enter an int, float, and string: ");
    minscanf("%d %f %s", &i, &f, s);
    printf("You entered: %d, %.2f, %s\n", i, f, s);
    return 0;
}