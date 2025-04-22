// Write a program escape(s,t) that convert newline and tab to visible chars like \n \t
#include <stdio.h>
#define MAXLINE 1000

void escape(char s[], char t[]) {
    int c, i, j = 0;
    for(i=0; (c=s[i]) != '\0'; i++) {
        switch (c) {
            
            case '\t':
                t[j++] = '\\';
                t[j++] = 't';
                break;
            case '\n':
                t[j++] = '\\';
                t[j++] = 'n';
                break;
            default:
                t[j++] = c;
                break;
        }
    }
    t[j] = '\0';
}

int main() {
    char s[] = "This is\ta test case!\n";
    char t[MAXLINE];
    escape(s, t);
    printf("%s\n", t);
    return 0;
}