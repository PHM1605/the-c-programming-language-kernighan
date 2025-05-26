// type a long string (could be new line included)-> save to an array of ints
#include <ctype.h>
#include <stdio.h>

#define SIZE 100
#define BUFSIZE 100

int buf[BUFSIZE];
int bufp = 0;

int getch(void) {
    return (bufp>0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
    if (bufp>=BUFSIZE)
        printf("ungetch: can't push characters\n");
    else 
        buf[bufp++] = c;
}

// get next integer from input to (saving to *pn); 
// return 0 if not an integer, 1 if a number 
int getint(int* pn) {
    int c, sign;
    while(isspace(c=getch()));
    if (!isdigit(c) && c!=EOF && c!='+' && c!='-') {
        ungetch(c); 
        return 0; // stop the process from now till the end of SIZE array
    }

    sign = (c=='-') ? -1 : 1;
    if (c=='+' || c=='-') {
        int sign_char = c;
        c = getch();
        if (!isdigit(c)) {
            ungetch(c);
            ungetch(sign_char);
            return 0;
        }
    }
    
    for (*pn=0; isdigit(c); c=getch()) {
        *pn = 10* *pn + (c-'0');
    }
    *pn *= sign;

    if (c!=EOF)
        ungetch(c);
    
    return c;
}

int main() {
    int n, array[SIZE], getint(int *);
    // fill the array
    for (n=0; n<SIZE && getint(&array[n]) != EOF; n++);
    for(int i =0; i< n; i++)
        printf("%d\n", array[i]);
    return 0;
}