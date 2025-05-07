// Replace tabs in the input with proper number of spaces (e.g. 1 tab = 3 spaces)
#include <stdio.h>
#define TABSTOP 3 
#define MAXLINE 1000

void detab(char line[], char detab_line[]) {
    int i, j=0;
    int n_insert = 0;
    for (i=0; line[i]!=EOF; i++) {
        if (line[i] == '\t') {
            n_insert = TABSTOP - (i % TABSTOP) + 1;
            while(n_insert-- > 0) {
                detab_line[j++] = ' ';
            }
        } else {
            detab_line[j++] = line[i];
        }
    }
}

int get_line(char line[]) {
    int c, i;
    for (i=0; i<MAXLINE-1 && (c=getchar()) != EOF && c!='\n'; ++i) {
        line[i] = c;
    }
    if (c == '\n') {
        line[i++] = c;
    }
    line[i] = '\0';
    return i;
}

int main() {
    char line[MAXLINE];
    char detab_line[MAXLINE];
    int len;
    while ((len=get_line(line)) > 0) {
        detab(line, detab_line);
        printf("%s", detab_line);
    }
    
    return 0;
}