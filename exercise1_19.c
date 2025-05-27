// Reverse a string
#include <stdio.h>
#define MAXLINE 1000

int get_line(char line[], int limit) {
    int i, c;
    for (i=0; i<limit-1 && (c=getchar())!=EOF && c!='\n'; ++i) {
        line[i] = c;
    }
    if (c=='\n') {
        line[i] = c;
        ++i; 
    }
    line[i] = '\0';
    return i;
}

int get_len(char line[]) {
    int i;
    for(i=0; line[i]!='\0'; ++i) {
    }
    return i;
}

void reverse_string(char line[], char reverse[]) {
    int len = get_len(line) - 1;
    int i;
    for (i=0; i<len; i++) {
        reverse[i] = line[len-i-1];
    }
    reverse[i] = '\n';
    reverse[i+1] = '\0';
}

int main() {
    int len;
    char line[MAXLINE];
    char reverse[MAXLINE];
    while((len=get_line(line, MAXLINE))>0) {
        reverse_string(line, reverse);
        printf("%s", reverse);
    }
    return 0;
}