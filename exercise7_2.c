#include <stdio.h>
#include <ctype.h>
#define MAX_LINE_WIDTH 80 // break lines if exceed this

int main() {
    int c;
    int col = 0;
    while((c=getchar()) != EOF) {
        // check if a character is printable
        if (isprint(c) || c == ' ' || c == '\t') {
            putchar(c);
            col++;
        } else if (c=='\n') {
            putchar(c);
            col = 0;
        } else {
            printf("\\%03o", c); // minimum field width is 3, octal
            col += 4;
        }

        // break line if exceeds max column width
        if (col >= MAX_LINE_WIDTH) {
            putchar('\n');
            col=0;
        }
    }
}
