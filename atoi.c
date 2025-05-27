/* atoi
- skip white space, if any
- get sign, if any
- get integer part and convert it
Library uses 'strtol' to convert from string to long-int
*/
#include <stdio.h>
#include <ctype.h>

int atoi(char s[]){
    int i, n, sign; // n: for storing integer part 
    // skipping white spaces at the beginning
    for (i=0; isspace(s[i]); i++) {}
    // sign
    sign = (s[i] == '-'? -1: 1);
    // get integer part 
    if (s[i]=='+' || s[i]=='-') {
        i++;
    }
    for (n=0; isdigit(s[i]); i++) {
        n = 10*n + (s[i]-'0');
    }
    return sign * n;
}

int main() {
    char s[] = "  -32";
    printf("%d\n", atoi(s)); // should be -32
    return 0;
}