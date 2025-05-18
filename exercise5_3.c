// Write a pointer version of strcat(s,t), copying t to the end of s
#include <stdio.h>

void mstrcat(char* s, char* t) {
    int i, j;
    i = j = 0;
    // move to the end of s
    while(*(s+i) != '\0') {
        i++;
    }
    // copying
    while((*(s+i) = *(t+j)) != '\0') {
        i++;
        j++;
    }
}

 int main() {
    char s[] = "abcd";
    char t[] = "ef";
    mstrcat(s, t);
    printf("Concatenated string: %s\n", s);
    return 0;
 }