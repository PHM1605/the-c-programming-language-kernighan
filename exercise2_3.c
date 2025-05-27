// Write a function htoi(s), which converts a string of hex digits (including optional 0x or 0X) into its equivalent integer value
#include <stdio.h>
#include <ctype.h>

int htoi(const char s[]) {
    int i = 0; // index of input string
    int digit;
    int result = 0;
    if (s[0] == '0' && (s[1]=='x' || s[1]=='X')) {
        i=2;
    }
    for (; s[i]!='\0'; ++i) {
        if (isdigit(s[i])) {
            digit = s[i] - '0';
        } else if (s[i] >= 'a' && s[i] <= 'f') {
            digit = s[i] - 'a' + 10;
        } else if (s[i] >= 'A' && s[i] <= 'F') {
            digit = s[i] - 'A' + 10;
        } else {
            printf("Error: Invalid hex digit '%c'\n", s[i]);
            return -1;
        }
        // NOTICE: write a number on the right == current_number*base+ that_number
        result = result*16+digit;
    }
    return result;
}

int main() {
    printf("0x1A3F -> %d\n", htoi("0x1A3F"));
    printf("7F -> %d\n", htoi("7F"));
    printf("0XdeadBEEF -> %d\n", htoi("0XdeadBEEF"));
    printf("invalid -> %d\n", htoi("0x12G4"));
    return 0;
}