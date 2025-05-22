// add a field-searching, so sorting may be done on fields within lines
// each field sorted according to an independent sort of options
// gcc exercise5_17.c 
// ./a.out -k2 -n -r < exercise5_17_input.txt
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLEN 1000 // max length of 1 line
#define MAXLINES 5000 // max #lines to be sorted
char* lineptr[MAXLINES]; // list of pointers to lines

// buffer for alloc
#define ALLOCSIZE 5000
static char allocbuf[ALLOCSIZE];
static char* allocp = allocbuf;

int numeric = 0;
int reverse = 0;
int fold = 0;
int directory = 0;
int key_field = 0; // 0 = whole line, >0 = field number

char* alloc(int n) {
    if (allocbuf + ALLOCSIZE - allocp >= n) {
        allocp += n;
        return allocp - n;
    } else {
        return 0;
    }
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

int readlines(char* lineptr[], int maxlines) {
    int len; // length of 1 line
    int nlines = 0; // #lines so far
    char line[MAXLEN];
    char* p;
    while((len = mgetline(line, MAXLEN)) > 0) {
        if (line[len-1] == '\n')
            line[len-1] = '\0'; // delete '\n'
        if (nlines >= maxlines || (p=alloc(len+1)) == NULL)
            return -1;
        
        strcpy(p, line);
        lineptr[nlines++] = p;
    }
    return nlines;
}

void writelines(char* lineptr[], int nlines) {
    // printf("%s", lineptr[0]);
    for (int i=0; i<nlines; i++) {
        printf("%s\n", lineptr[i]);
    }
}

char* get_field(char* line, int field_num) {
    // printf("getfield-S: %s\n", line);
    
    int count = 0; // which field number on a line we are processing?
    while(*line) {
        
        // consume white spaces
        while(*line && isspace((unsigned char)* line)) line++;
        if (*line == '\0') break;
        // start of line with first char
        char* start = line;
        // move to the end of field 
        while(*line && !isspace((unsigned char)* line)) line++;
        count++; // as field counting from 1, not 0
        if (count == field_num) {
            char* field = (char*) malloc(MAXLEN); // field being processed
            int len = line - start;
            strncpy(field, start, len);
            field[len] = '\0';
            printf("field return: %s\n", field);
            return field;
        }
    }
    return NULL; // field not found
}

// a here is a pointer to a pointer to string e.g. "23"
int numcmp(void* a, void* b) {
    char* s1 = *(char **)a;
    char* s2 = *(char **)b;
    char* field1;
    char* field2;
    
    if (key_field > 0) {
        field1 = get_field(s1, key_field);
        field2 = get_field(s2, key_field);
        double v1 = atof(field1);
        double v2 = atof(field2);
        if (field1 != NULL)
            free(field1);
        if (field2 != NULL)
            free(field2);
        if (v1 < v2) return reverse ? 1: -1;
        if (v1 > v2) return reverse ? -1 : 1;
        return 0;
    } else {
        printf("ERROR: field must be >=1\n");
        return -99;
    }
    
}

// same signature as above, but to a string e.g."dog"
int mstrcmp(void* a, void* b) {
    char* s1 = *(char**)a;
    char* s2 = *(char**)b;
    char* field1;
    char* field2;
    if (key_field > 0) {
        field1 = get_field(s1, key_field);
        field2 = get_field(s2, key_field);
        for(; *field1 && *field2; field1++, field2++) {
            char c1 = fold ? tolower((unsigned char)*field1) : *field1;
            char c2 = fold ? tolower((unsigned char)*field2) : *field2;
            if (c1 != c2)
                return reverse ? c2-c1 : c1-c2;
        }
        return reverse ? *field2 - *field1 : *field1- *field2;
    } else {
        printf("ERROR: field must be >=1\n");
        return -99;
    }    
}

void swap(void* v[], int i, int j) {
    void* temp; 
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

// qsort: sort v[left]...v[right] into increasing order
void mqsort(void* v[], int left, int right, int (*comp)(void*, void*)) {
    int i, last;
    if (left >= right)
        return;
    swap(v, left, (left+right)/2);
    last = left;
    for (i=left+1; i<=right; i++) {
        if ((*comp)(&v[i], &v[left]) < 0) {
            printf("SWAP\n");
            swap(v, ++last, i);
        } else {
            printf("NO SWAP\n");
        }
            
    }
    swap(v, left, last);
    mqsort(v, left, last-1, comp);
    mqsort(v, last+1, right, comp);
}

int main(int argc, char* argv[]) {
    int nlines; // number of input lines read 

    for (int i=1; i<argc; i++) {
        printf("\n%s\n", argv[i]);
        if (strcmp(argv[i], "-n") == 0)
            numeric = 1;
        else if (strcmp(argv[i], "-r") == 0)
            reverse = 1;
        else if (strcmp(argv[i], "-f") == 0)
            fold = 1;
        // compare the first 2 keys of e.g. "-k3" only
        else if (strncmp(argv[i], "-k", 2) == 0)
            // argv[i]+2 points to the '3' char -> key_field=3
            key_field = atoi(argv[i] + 2);
    }
    
    printf("%d", key_field);
    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        mqsort((void**)lineptr, 0, nlines-1, numeric? numcmp: mstrcmp);
        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("input too big to sort\n");
        return 1;
    }
}   
