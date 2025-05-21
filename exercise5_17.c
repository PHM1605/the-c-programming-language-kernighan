// add a field-searching, so sorting may be done on fields within lines
// each field sorted according to an independent sort of options
// gcc exercise5_17.c 
// ./a.out -k2n -r < exercise5_17_input.txt
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

int fold = 0;
int directory = 0;
int key_field = 0; // 0 = whole line, >0 = field number

// qsort: sort v[left]...v[right] into increasing order
void mqsort(void* v[], int left, int right, int (*comp)(void*, void*), int reverse) {
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
        int cmp = (*comp)(v[i], v[left]);
        if ((!reverse && (cmp<0)) || (reverse && (cmp>0)))
            swap(v, ++last, i);
    }
    swap(v, left, last);
    mqsort(v, left, last-1, comp, reverse);
    mqsort(v, last+1, right, comp, reverse);
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

int mstrcmp_fold(char* s1, char* s2) {
    for(; *s1 && *s2; s1++, s2++) {
        char c1 = tolower((unsigned char)* s1);
        char c2 = tolower((unsigned char)* s2);
        if (c1 != c2)
            return c1 - c2;
    }
    // return 0 only when both end chars = '\0'
    return tolower((unsigned char)* s1) - tolower((unsigned char)* s2);
}

int is_directory_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c);
}

int mstrcmp_fold_dir(char* s1, char* s2) {
    char c1, c2;
    
    for(;;s1++, s2++) {
        // skip non-directory chars in s1 and s2
        while(*s1 && !is_directory_char(*s1))
            s1++;
        while(*s2 && !is_directory_char(*s2))
            s2++;
        
        c1 = fold ? tolower((unsigned char)*s1) : *s1;
        c2 = fold ? tolower((unsigned char)* s2) : *s2;

        // break if they start to differ or reach the end
        if (c1!=c2 || c1=='\0' || c2=='\0')
            break;
    }
    return c1 - c2;
}

char* get_field(char* line, int field_num) {
    static char field[MAXLEN]; // field being processed
    int count = 0; // which field number on a line we are processing?
    while(*line) {

    }
    return ""; // field not found
}

int main(int argc, char* argv[]) {
    int nlines; // number of input lines read 
    int numeric = 0; // 0 if strcmp, 1 if numeric sort
    int reverse = 0; // 0: ascending; 1: descending

    for (int i=1; i<argc; i++) {
        if (strcmp(argv[i], "-n") == 0)
            numeric = 1;
        else if (strcmp(argv[i], "-r") == 0)
            reverse = 1;
        else if (strcmp(argv[i], "-f") == 0)
            fold = 1;
        else if (strcmp(argv[i], "-d") == 0)
            directory = 1;
        // compare the first 2 keys of e.g. "-k3" only
        else if (strncmp(argv[i], "-k", 2) == 0)
            // argv[i]+2 points to the '3' char -> key_field=3
            key_field = atoi(argv[i] + 2);
    }
    
    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        mqsort((void**)lineptr, 0, nlines-1, numeric? numcmp: mstrcmp);
        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("input too big to sort\n");
        return 1;
    }

}   

// ./sortlines -k2n -r

// → Sort by 2nd field, numeric, reverse order

// mqsort((void**)lineptr, 0, nlines-1, numeric ? numcmp : mstrcmp, reverse);

// int numcmp(void* a, void* b) {
//     char* s1 = *(char**)a;
//     char* s2 = *(char**)b;

//     if (key_field > 0) {
//         s1 = get_field(s1, key_field);
//         s2 = get_field(s2, key_field);
//     }

//     double v1 = atof(s1);
//     double v2 = atof(s2);

//     if (v1 < v2) return reverse ? 1 : -1;
//     if (v1 > v2) return reverse ? -1 : 1;
//     return 0;
// }

// int mstrcmp(void* a, void* b) {
//     char* s1 = *(char**)a;
//     char* s2 = *(char**)b;

//     if (key_field > 0) {
//         s1 = get_field(s1, key_field);
//         s2 = get_field(s2, key_field);
//     }

//     if (fold) {
//         while (*s1 && *s2) {
//             char c1 = tolower((unsigned char)*s1);
//             char c2 = tolower((unsigned char)*s2);
//             if (c1 != c2)
//                 return reverse ? c2 - c1 : c1 - c2;
//             s1++; s2++;
//         }
//         return reverse ? tolower(*s2) - tolower(*s1) : tolower(*s1) - tolower(*s2);
//     }

//     int res = strcmp(s1, s2);
//     return reverse ? -res : res;
// }

// char* get_field(char* line, int field_num) {
//     static char field[MAXLEN];
//     int count = 0;

//     while (*line) {
//         // Skip leading spaces
//         while (*line && isspace((unsigned char)*line)) line++;

//         if (*line == '\0') break;

//         char* start = line;

//         while (*line && !isspace((unsigned char)*line))
//             line++;

//         count++;

//         if (count == field_num) {
//             int len = line - start;
//             strncpy(field, start, len);
//             field[len] = '\0';
//             return field;
//         }
//     }

//     return ""; // field not found
// }