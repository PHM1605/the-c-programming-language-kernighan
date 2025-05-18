// Write strend(s,t) returning 1 if string t at the end of s, 0 otherwise
#include <string.h>
#include <stdio.h>

int strend(char s[], char t[]) {
    int i;
    int len_s = strlen(s);
    int len_t = strlen(t);
    while(len_s>0 && len_t>0 && s[len_s-1] == t[len_t-1]) {
        len_s--;
        len_t--;
    }

    return len_t==0;
}

int main() {
    char s[] = "abcdef";
    char t1[] = "def";
    char t2[] = "dsf";
    
    printf("t1 at the end of s? %d\n", strend(s, t1));
    printf("t2 at the end of s? %d\n", strend(s, t2));
}