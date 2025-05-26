// gcc pointer_to_function.c -o pointer_to_function
// ./pointer_to_function -n < pointer_to_function.txt
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLEN 1000 // max length of 1 line
#define MAXLINES 5000 // max #lines to be sorted
char* lineptr[MAXLINES]; // list of pointers to lines

// buffer for alloc
#define ALLOCSIZE 5000
static char allocbuf[ALLOCSIZE];
static char* allocp = allocbuf;

// qsort: sort v[left]...v[right] into increasing order
void mqsort(void* v[], int left, int right, int (*comp)(void*, void*)) {
    int i, last;

    void swap(void* v[], int i, int j) {
        void* temp; 
        temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }

    if (left >= right)
        return;
    swap(v, left, (left+right)/2);
    last = left;
    for (i=left+1; i<=right; i++) {
        if ((*comp)(v[i], v[left]) < 0)
            swap(v, ++last, i);
    }
    swap(v, left, last);
    mqsort(v, left, last-1, comp);
    mqsort(v, last+1, right, comp);
}

int mgetline(char* s, int lim) {
    int c, i=0;
    while(--lim>0 && (c=getchar()) != EOF && c!='\n')
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

int readlines(char* lineptr[], int maxlines) {
    int len; // length of 1 line
    int nlines = 0; // #lines so far
    char line[MAXLEN];
    char* p;
    while((len = mgetline(line, MAXLEN)) > 0) {
        if (nlines >= MAXLINES || (p=alloc(len)) == NULL)
            return -1;
        else {
            line[len-1] = '\0'; // delete '\n'
            strcpy(p, line);
            lineptr[nlines++] = p;
        }
    }
    return nlines;
}

void writelines(char* lineptr[], int nlines) {
    // printf("%s", lineptr[0]);
    for (int i=0; i<nlines; i++) {
        printf("%s\n", lineptr[i]);
    }
}

// s1: "12", s2: "43"
int numcmp(char* s1, char* s2) {
    double v1, v2;
    v1 = atof(s1);
    v2 = atof(s2);
    if (v1 < v2)
        return -1;
    else if (v1 > v2)
        return 1;
    else   
        return 0;
}

// as strcmp arguments are (const char*, const char*)
int mstrcmp(char* s1, char* s2) {
    return strcmp((char*)s1, (char*)s2);
}

int main(int argc, char* argv[]) {
    int nlines; // number of input lines read 
    int numeric = 0; // 0 if strcmp, 1 if numeric sort
    if (argc>1 && strcmp(argv[1], "-n") == 0)
        numeric = 1;
    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        mqsort((void**) lineptr, 0, nlines-1, (int (*)(void*,void*))(numeric? numcmp: mstrcmp));
        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("input too big to sort\n");
        return 1;
    }

}   