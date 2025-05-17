#include <stdio.h>
#include <ctype.h>
#include "calc.h"

// Get next character or numeric operand
int getop(char s[]) {
    int i, c; // i: index of s; c: to store the typed-in character
    while((s[0] = c = getch()) == ' ' || c=='\t');
    s[1] = '\0';
    // not a number
    if (!isdigit(c) && c!='.') {
        return c; 
    }
    i=0;
    // collect integer part
    if (isdigit(c)) {
        while(isdigit(s[++i] = c = getch()));
    }
    // collect fraction part
    if (c=='.')
        while(isdigit(s[++i] = c = getch()));
    s[i] = '\0';
    if (c!=EOF)
        ungetch(c); 
    return NUMBER;
}