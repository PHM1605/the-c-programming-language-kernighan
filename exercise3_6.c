// Write a version of itoa that accepts 3 arguments; 3rd one is minimum field width (should be padding with zeros if less)
#include <stdio.h>
#include <string.h>

void reverse(char s[]){
    int i, j;
    int tmp;
    for(i=0, j=strlen(s)-1; i<j; i++, j--) {
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
    }
}

void itoa_with_pad(int number, char s[], int width) {
    int i=0;
    int sign, tmp;
    // handle negative numbers
    if ((sign=number) < 0) {
        number = -number;
    }
    do {
        s[i++] = number%10 + '0';
    } while((number/=10) > 0);

    while (i<width) {
        s[i++]='0';
    }

    if (sign<0) {
        s[i++] = '-';
    }
    s[i] = '\0';
    reverse(s);
}

int main() {
    char s[100];
    itoa_with_pad(34, s, 8);
    printf("%s\n", s); // should be "00000034"
    return 0;
}