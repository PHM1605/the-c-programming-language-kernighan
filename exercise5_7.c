// Same as 'array_of_pointers', but store lines in an array supplied by main, rather than calling 'alloc'

#include <stdio.h>
#include <string.h>

#define MAXLEN 1000 // max length of any input line
#define MAXLINES 5000 // max number of lines 


char* lineptr[MAXLINES];
char linebufs[MAXLINES][MAXLEN];

int mgetline(char* s, int lim) {
    int c, i=0;
    while(--lim>0 && (c=getchar()) != EOF && c!='\n')
        s[i++] = c;
    if (c=='\n')
        s[i++] = c;
    s[i] = '\0';
    return i;
}

int readlines(char* lineptr[], char buf[][MAXLEN], int maxlines) {
    int len; // length of 1 line
    int nlines = 0; // #lines so far
    char line[MAXLEN];
    while((len=mgetline(line, MAXLEN))>0) {
        if (nlines >= maxlines) 
            return -1;
        line[len-1] = '\0'; // delete the '\n'
        strcpy(buf[nlines], line);
        lineptr[nlines] = buf[nlines];
        nlines++;
    }
    return nlines;
}

void writelines(char* lineptr[], int nlines) {
    for(int i=0; i<nlines; i++) {
        printf("%s\n", lineptr[i]);
    }
}

void qsort(char* v[], int left, int right) {
    int i, last;

    void swap(char* v[], int i, int j) {
        char* temp;
        temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }
    if (left >= right) return;
    swap(v, left, (left+right)/2);
    last = left;
    for (i=left+1; i<=right; i++) {
        if (strcmp(v[i], v[left]) < 0)
            swap(v, ++last, i);
    }
    swap(v, left, last);
    qsort(v, left, last-1);
    qsort(v, last+1, right);
}

int main() {
    int nlines;
    if ((nlines = readlines(lineptr, linebufs, MAXLINES)) >=0) {
        qsort(lineptr, 0, nlines-1);
        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("error: input is too big to sort\n");
        return 1;
    }
}