// Write strindex(s,t) that returns the position of the rightmost occurance of t in s, or -1 if there is none
#include <stdio.h>

int strindex(char s[], char t[]) {
    int i, j, k;
    int ret = -1;
    for (i=0; s[i]!='\0'; i++) {
        for (j=i, k=0; t[k]!='\0' && s[j]==t[k]; j++, k++) {}
        if (k>0 && t[k]=='\0') {
            ret = i;
        }
    }
    return ret;
}

int main() {
    char s[] = "This is test";
    char t[] = "is";
    printf("%d\n", strindex(s, t)); // should show 2nd 'is' location i.e. 5, not 2
    return 0;
}