// Write 'getfloat()', typing a long string, save to array of double
#include <stdio.h>
#include <ctype.h>

#define MAXSIZE 100
int buf[100];
int bufp = 0;

int getch(void) {
    return (bufp>0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
    if (bufp < sizeof(buf))
        buf[bufp++] = c;
}

int getfloat(double* pn) {
    int c, sign;
    double power = 1.0;
    while(isspace(c=getch())); // skip white space
    if (!isdigit(c) && c!=EOF && c!='+' && c!='-'){
        ungetch(c);
        return 0;
    }
    sign = (c=='-') ? -1 : 1;
    if (c=='+' || c=='-') {
        c = getch();
    }
    // Integer part 
    for (*pn=0; isdigit(c); c=getch()) {
        *pn = 10* *pn + (c-'0');
    }
    if (c=='.') {
        c = getch();
        while(isdigit(c)) {
            *pn = 10.0 * *pn + (c-'0');
            power *= 10.0;
            c = getch();
        }
    }
    *pn = sign * (*pn/power);
    if (c!=EOF)
        ungetch(c);
    return c;
}

int main() {
    double array[MAXSIZE];
    int n = 0, c;
    printf("Enter up to %d floats (ctrl-D or non-number to stop):\n", MAXSIZE);
    while (n<MAXSIZE && (c=getfloat(&array[n])) != 0 && c!=EOF) {
        n++;
    }
    printf("\nRead %d float%s:\n", n, n==1?"":"s");
    for (int i=0; i<n; i++) {
        printf(" [%2d] = %g\n", i, array[i]);
    }

    return 0;
}