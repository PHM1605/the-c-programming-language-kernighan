// Write a program to replace multiple spaces by minimum number of tabs and spacing
#include <stdio.h>
#define MAXLINE 1000
#define TABSTOP 3

int get_line(char line[]) {
    int i, c;
    for(i=0; i<MAXLINE-1 && (c=getchar()) != EOF && c!='\n'; ++i) {
        line[i] = c;
    }
    if (c=='\n') {
        line[i++] = c;
    }
    line[i] = '\0';
    return i;
}

void entab(char line[], char tab_line[]) {
    int count = 0;
    int i, j = 0; // index for tab_line
    for (i=0; line[i]!='\0' && line[i]!='\n'; i++) {
        if (line[i] == ' ') {
            count++;
            if (count==TABSTOP) {
                tab_line[j++] = 't'; // showing '\t' with 't' for brevity
                count = 0;
            }
        } else {
            if (count>0) {
                for (int i=0; i<count; i++) {
                    tab_line[j++]='s'; // showing ' ' with 's' for brevity
                }
                count = 0;
            }            
            tab_line[j++] = line[i];
        }
    }
    if (line[i] == '\n') {
        tab_line[j++] = '\n';
    }
    
    tab_line[j] = '\0';
}

int main() {
    char line[MAXLINE];
    char tab_line[MAXLINE];
    int len;
    while((len=get_line(line)) > 0) {
        entab(line, tab_line);
        printf("%s", tab_line);
    }
    return 0;
}
