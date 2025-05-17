// Print an integer as a character string
#include <stdio.h>

#define MAXLEN 100

void my_itoa_recursive(int n, char s[], int* si) {
    if (n/10)
        my_itoa_recursive(n/10, s, si);
    s[(*si)++] = n%10+'0';
}

void my_itoa(int n, char s[]) {
    int si = 0;
    if (n<0) {
        s[si++] = '-';
        n = -n;
    }
    my_itoa_recursive(n, s, &si); // address to keep the value of si
    s[si] = '\0';
}

int main() {
    char s[MAXLEN];
    my_itoa(123, s);
    printf("%s", s);
    return 0;
}