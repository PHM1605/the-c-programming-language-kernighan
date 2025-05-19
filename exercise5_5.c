// Write strncpy(), strncat(), strncmp() which operate on at most the first n chars of the argument's strings
#include <stdio.h>

#define MAXSIZE 100

// strncat(s,t,n) concatenates at most n chars of t to s
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

// strncpy(s,t,n) copies at most n chars of t to s
void mstrncpy(char* s, char* t, int n) {
    int i =0;
    while( (*(s+i) = *(t+i)) && n>0) {
        i++;
        n--;
    }
    if (*(t+i) != '\0')
        *(s+i) = '\0';
}

// strncmp(s,t,n) compares the first n chars only
int mstrncmp(char* s, char* t, int n) {
    while(n-->0) {
        if (*s != *t) {
            return *s - *t;
        }
        // both are '\0' here
        if (*s=='\0') {
            return 0;
        }
        s++; 
        t++;
    }
    
    return 0; // two strings the same
}

int main() {
    char s[MAXSIZE] = "abcd";
    char t1[] = "efg";
    char t2[] = "egg"; 
    mstrncat(s, t1, 2); // s should be abcdef
    mstrncat(s, t2, 2); // s should be abcdefegg (as t2 only 1 char but we append 2 chars)
    printf("Concatenated string: %s\n", s);
    mstrncpy(s, t1, 2); // s should be 'ef'
    printf("Copied string: %s\n", s);
    printf("t1 and t2 the same for the first 1 character? %d\n", mstrncmp(t1, t2, 1)==0);
    printf("t1 and t2 the same for the first 3 character? %d\n", mstrncmp(t1, t2, 3)==0);
    return 0;
}