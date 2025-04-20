// Write any(s1, s2) which returns the first location in s1 where any char in string s2 occurs (return -1 if not exist)
// from stdlib using 'strpbrk', but return a pointer to that location instead
#include <stdio.h>

int any(char s1[], char s2[]) {
    int loc = -1;
    int c1, c2; 
    int i=0, j=0;
    for(i=0; s2[i] != '\0'; i++) {
        for(j=0; s1[j] != '\0'; j++) {
            if (s1[j] == s2[i]) {
                break;
            }
        }
        if ((loc == -1 && s1[j] != '\0') || j<loc) {
            loc = j;
        }
    }
    return loc;
}

int main() {
    char s1[] = "This is a test string";
    char s2[] = "it";
    char s3[] = "xyz";
    printf("%d\n", any(s1, s2)); // should be 2 i.e. the location of 'i'
    printf("%d\n", any(s1, s3)); // should be -1
    return 0;
}