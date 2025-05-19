#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXSIZE 100
// A pointer version of getline
int mgetline(char* s, int lim) {
    int c, i=0;
    char* start = s;
    while(--lim>0 && (c=getchar()) != EOF && c!='\n')
        *s++ = c;
    if (c=='\n')
        *s++ = c;
    *s = '\0';
    return s-start;
}

// A pointer version of atoi
int matoi(char* s) {
    int sign, n=0;
    while(isspace(*s)) {
        s++;
    }
    sign = (*s=='-') ? -1 : 1;
    if (*s=='+' || *s=='-') 
        s++;
    while(isdigit(*s)) {
        n = 10*n + (*s-'0');
        s++;
    }
    return sign * n;
}

void mreverse(char* s) {
    int len = strlen(s);
    char* end = s+len-1;
    int c;
    while(s <= end) {
        c = *s;
        *s = *end;
        *end = c;
        s++;
        end--; 
    }
}

// A pointer version of itoa
void mitoa(int n, char* s) {
    int i, sign;
    char* start = s;
    if((sign=n) < 0) {
        n = -n;
    }
    do {
        *s++ = n%10+'0';
    } while((n/=10)>0);
    if (sign<0) 
        *s++ = '-';
    *s = '\0';
    mreverse(start);
}

// A pointer version of strindex (index of t in s)
int mstrindex(char* s, char* t) {
    int i, j, k;
    for (i=0; *(s+i)!='\0'; i++) {
        for (j=i, k=0; *(t+k)!='\0' && *(s+j)==*(t+k); j++, k++);
        if (k>0 && *(t+k)=='\0')
            return i;
    }
    return -1;
}

// A pointer version of getop (reading operand to s)
#define NUMBER '0'
#define MAXOP 100
#define BUFSIZE 100
char buf[BUFSIZE];
int bufp = 0;
int getch() {
    return (bufp>0) ? buf[--bufp] : getchar();
}
void ungetch(int c) {
    if (bufp >=BUFSIZE)
        printf("ungetch: too many chars\n");
    else 
        buf[bufp++] = c;
}
int getop(char* s) {
    int i, c;
    // consume spaces/tabs
    while((*s = c = getch()) == ' ' || c=='\t');
    *++s = '\0';
    if (!isdigit(c) && c!='.')
        return c;
    
    // collect integer part
    if (isdigit(c)) {
        while(isdigit(*s++ = c = getch()));
    }
    if (c=='.') {
        while(isdigit(*s++ = c = getch()));
    }
    *s = '\0';
    if (c!=EOF)
        ungetch(c);
    return NUMBER;
}

int main() {
    // // Check mgetline()
    // char line[MAXSIZE];
    // int len;
    // printf("Type in a line: ");
    // len=mgetline(line, MAXSIZE);
    // printf("Line has %d chars.\n", len);

    // // Check reverse
    // char test_reverse[] = "abc";
    // mreverse(test_reverse);
    // printf("Test reverse: %s\n", test_reverse);

    // // Check atoi
    // char num_str[] = "123";
    // int num_int = matoi(num_str);
    // printf("Test atoi: %d\n", num_int);

    // // // Check itoa
    // char itoa_res[MAXSIZE];
    // mitoa(num_int, itoa_res);
    // printf("Test itoa: %s\n", itoa_res);

    // // Check strindex
    // char s[] = "abcdef";
    // char t[] = "cd";
    // printf("Test strindex: %d\n", mstrindex(s, t)); // 2

    // Check getop
    int type;
    char s[MAXOP];
    printf("Check getop():\n");
    while ((type=getop(s)) != EOF) {
        printf("%d\n", type);
    }

    return 0;
}