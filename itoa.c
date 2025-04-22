// Convert an integer n to string
#include <stdio.h>
#include <string.h>
#define MAXLINE 1000

void reverse(char s[]) {
    int i, j;
    int c;
    for (i=0, j=strlen(s)-1; i<j; i++, j--) {
        c= s[i],
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int n, char s[]) {
    int i, sign;
    // handle negative number
    if ((sign=n) < 0) {
        n = -n;
    }
    i = 0;
    do {
        s[i++] = n%10 + '0';
    } while((n/=10) > 0);

    if (sign < 0)
        s[i++] = '-';

    s[i] = '\0';
    reverse(s);
}

int main() {
    int number = -29;
    char number_str[MAXLINE];
    itoa(number, number_str);
    printf("%s\n", number_str);
    return 0;
}