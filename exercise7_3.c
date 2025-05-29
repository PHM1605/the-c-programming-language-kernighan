// minimal printf with variable argument list
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void minprintf(char* fmt, ...) {
    va_list ap; // points to each unnamed arg in turn
    char* p; // pointer of format (fmt)
    char subfmt[100]; // to hold the sub-format
    char* subfmt_ptr; // pointer of sub-format
    int ch;

    va_start(ap, fmt); // make ap point to 1st unnamed arg

    for (p=fmt; *p; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }

        // copy the '%' into sub-format
        subfmt_ptr = subfmt;
        *subfmt_ptr++ = *p++;

        // copy the middle part of e.g. %.2f (the '.2' part)
        // strchr: return true if the char '*p' is part of a long string
        while(strchr("+-# 0123456789.lh", *p)) {
            *subfmt_ptr++ = *p++;
        }
        // copy the final char of e.g. %.2f i.e. 'f'
        *subfmt_ptr++ = *p;
        *subfmt_ptr = '\0';

        switch(*p) {
        case 'd': case 'i': case 'u': case 'x': case 'X': case 'o':
            // va_arg: process ap and move to the next argument
            printf(subfmt, va_arg(ap, int));
            break;
        case 'f': case 'F': case 'e': case 'E': case 'g': case 'G':
            printf(subfmt, va_arg(ap, double));
            break;
        case 'c':
            ch = va_arg(ap, int);
            printf(subfmt, ch);
            break;
        case 's':
            printf(subfmt, va_arg(ap, char*));
            break;
        case 'p':
            printf(subfmt, va_arg(ap, void*));
            break;
        case '%':
            putchar('%'); // handle %%
            break;
        default:
            // print unknown format specifier as-is
            fputs(subfmt, stdout);
            break;
    
        }
    }
    va_end(ap); // clean-up ap when done
}

int main() {
    char* s = "World";
    // #x means printing also the '0x' part of that hex number
    minprintf("Int: %d, Hex: %#x, Float: %.2f, String: %s\n", 42, 42, 3.1415, "hello");
    return 0;
}