// Write squeeze(s1, s2) that deletes s1 that matches any chars in s2
#include <stdio.h>
#include <string.h>
#define MAXLINE 1000

void squeeze(char s1[], char s2[]) {
    int i, j, k;
    int c;
    for (i=0; (c=s2[i])!='\0'; i++) {
        // remove char c in s2 from s1
        for (j=k=0; s1[j] != '\0'; j++) {
            if (s1[j] != c) {
                s1[k++] = s1[j];
            }
        }
        s1[k] = '\0';
    }
}

int main() {
    char s1[MAXLINE] = "test";
    char s2[MAXLINE] = "es";
    squeeze(s1, s2);
    printf("%s", s1); // should be "tt"
    return 0;
}