// Write a program to fold long input line to >=2 shorter lines exceeding #cols
#include <stdio.h>
#define MAXCOL 20
#define MAXLINE 1000

int get_line(char line[]) {
    int c, i = 0;
    while((c=getchar()) != EOF && c!='\n' && i<MAXLINE-1) {
        line[i++] = c;
    }
    if (c=='\n')
        line[i++] = '\n';
    line[i] = '\0';
    return i;
}

void fold_line(char line[], char folded_line[]) {
    int c,i=0, j=0;
    int marked_loc = 0; // mark the beginning of the last space
    int line_start_loc = 0; // where we insert the '\n'
    for (i=0; (c=line[i])!=EOF && line[i]!='\n'; ++i) {
        folded_line[j++] = c;
        if (c==' ') {
            marked_loc = i;
        } else if (i-line_start_loc>=MAXCOL) {
            folded_line[marked_loc] = '\n';
            line_start_loc= marked_loc+1;
        }
    }
    if (c=='\n')
        folded_line[j++] = '\n';
    folded_line[j] = '\0';
}

int main() {
    char line[MAXLINE];
    char folded_line[MAXLINE];
    int len;
    while((len = get_line(line)) > 0) {
        fold_line(line, folded_line);
        printf("%s", folded_line);
    }
    
    return 0;
}
