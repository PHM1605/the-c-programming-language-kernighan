// Write itob(n, s, b) (integer to base) e.g. itob(n, s, 16) will format s a hex integer in s
#include <stdio.h>
#include <string.h>

void reverse(char s[]) {
    int i, j;
    char temp;
    for (i=0, j=strlen(s)-1; i<j; i++, j--) {
        temp = s[i];
        s[i] = s[j];
        s[j] = temp;
    }
}

void itob(int n, char s[], int b) {
    int i=0;
    int sign = (n>=0) ? 1 : -1;
    unsigned int num;
    // handle negative numbers for base 10 only
    if (n<0 && b == 10) {
        num = -n;
    } else  {
        num = (unsigned) n;
    }

    do {
        int digit = num%b;
        s[i++] = (digit<10) ? digit + '0' : digit-10+'A';
    } while((num/=b) > 0);

    if (sign<0 && b==10) {
        s[i++] = '-';
    }
    
    s[i] = '\0';
    reverse(s);
}

int main() {
    char s[100];
    itob(155, s, 16);
    printf("155 in hex: %s\n", s);
    itob(-123, s, 10);
    printf("-123 in decimal: %s\n", s);
    itob(34, s, 2);
    printf("34 in binary: %s\n", s);
    itob(-345, s, 16);
    printf("-345 in hex (unsigned): %s\n", s);
    return 0;
}