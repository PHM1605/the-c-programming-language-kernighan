// Remove trailing blanks and tabs from each line of input, delete entirely blank lines
#include <stdio.h>
#define MAXLINE 1000

void remove_trailing_spaces(char line[]);
int get_line(char line[], int lim);

int main() {
    char line[MAXLINE];
    int len;
    
    while((len=get_line(line, MAXLINE)) > 0) {
        remove_trailing_spaces(line);
        if (line[0] != '\0') {
            printf("%s\n", line);
        }
    }
    
    return 0;
}

int get_line(char line[], int lim) {
    int i, c;
    for (i=0; i<lim-1 && (c=getchar()) != EOF && c!='\n'; ++i) {
        line[i] = c;
    }
    if (c=='\n') {
        line[i] = c;
        ++i;
    }
    line[i] = '\0';
    return i;
}

void remove_trailing_spaces(char line[]) {
    int i = 0;
    while(line[i] != '\0') {
        i++;
    }
    i--; // move to the last character before '\0'
    while(i>=0 && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')) {
        i--;
    }
    line[i+1] = '\0';
}
