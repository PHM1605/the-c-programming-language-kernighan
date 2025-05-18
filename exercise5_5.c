// Write strncpy(), strncat(), strncmp() which operate on at most the first n chars of the argument's strings
// strncat(s,t,n) concatenates at most n chars of t to s
// strncpy(s,t,n) copies at most n chars of t to s
#include <stdio.h>

void mstrncat(char* s, char* t, int n) {
    int i, j;
    i = j = 0;
    // move to the end of s
    while(*(s+i) != '\0') {
        i++;
    }
    while(n>0 && (*(s+i)=*(t+j)) !='\0') {
        i++;
        j++;
        n--;
    }
    s[i] = '\0';
}

int main() {
    char s[] = "abcd";
    char t1[] = "efg";
    char t2[] = "e"; 
    mstrncat(s, t1, 2); // s should be abcdef
    mstrncat(s, t2, 2); // s should be abcdefe (as t2 only 1 char but we append 2 chars)
    printf("Copied string: %s", s);

    return 0;
}