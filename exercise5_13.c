// 'tail -n' means printing the last n lines of input
// gcc exercise5_13.c -o tail
// ./tail.exe -2 < input.txt
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXOP 10
#define MAXLINES 5000 // max number of lines in file
#define MAXLEN 1000 // max length of any input line 
#define ALLOCSIZE 10000

static char allocbuf[ALLOCSIZE];
static char* allocp = allocbuf;

// stack for arg
char op[MAXOP];
int op_pos = 0;

int get_arg(char* start) {
    op_pos = 0;
    // skip the '-'
    while(*++start != '\0') {
        op[op_pos++] = *start;
    } 
    op[op_pos] = '\0';
}

int mgetline(char* s, int lim) {
    int c, i=0;
    while(--lim > 0 && (c=getchar()) != EOF && c!='\n')
        s[i++] = c;
    if (c=='\n')
        s[i++] = c;
    s[i] = '\0';
    return i;
}

char* alloc(int n) {
    if (allocbuf + ALLOCSIZE - allocp >= n) {
        allocp += n;
        return allocp - n;
    } else {
        return 0;
    }
}

int readlines(char* lineptr[]) {
    int len; // length of 1 line 
    int nlines = 0; // #lines so far
    char* p;
    char line[MAXLEN]; // containing 1 line
    while((len=mgetline(line, MAXLEN)) > 0) {
        if (nlines >= MAXLINES || (p = alloc(len)) == NULL)
            return -1;
        else {
            line[len-1] = '\0'; // delete newling
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    }
    return nlines;
}

int main(int argc, char* argv[]) {
    int n = 10;
    if (argc > 1) {
        if (argv[1][0] == '-') {
            get_arg(argv[1]);
            n = atoi(op);
        }
    }
    char* lineptr[MAXLINES];
    int nlines; // number of lines in the file
    if ( (nlines = readlines(lineptr)) >= 0) {
        for (int i_lines=nlines-n; i_lines<nlines && n>0; n--, i_lines++) 
            printf("%s\n", lineptr[i_lines]);
    } else {
        printf("ERROR: input is too big\n");
        return 1;
    }
    return 0;
}