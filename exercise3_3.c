// Write expand(s1, s2) that expands short-hand notation like a-z to abc...xyz. Allow either cases and digits, prepare for a-b-c, a-z0-9 and -a-z(i.e.-abc...z)
#include <stdio.h>
#include <string.h>

#define MAXLINE 1000

void expand(char s1[], char s2[]) {
    int i, k, j=0;
    int start, end;
    for (i=0; i<strlen(s1); i++) {
        if (i==0 || i==strlen(s1) || s1[i]!='-')
            s2[j++] = s1[i];
        else {
            start = s1[i-1];
            end = s1[i+1];
            for (k=start+1; k<end; k++) {
                s2[j++] = k;
            }
        }
    }
    s2[j] = '\0';
}

int main() {
    char res[MAXLINE];
    char s1[] = "a-z";
    char s2[] = "A-H";
    char s3[] = "a-b-c";
    char s4[] = "a-z0-9";
    char s5[] = "-a-z";
    expand(s1, res);
    printf("%s\n", res);
    expand(s2, res);
    printf("%s\n", res);
    expand(s3, res);
    printf("%s\n", res);
    expand(s4, res);
    printf("%s\n", res);
    expand(s5, res);
    printf("%s\n", res);
    return 0;
}