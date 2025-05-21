#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXLEN 1000
#define MAXLINES 5000
#define ALLOCSIZE 5000

char *lineptr[MAXLINES];
static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;

int numeric = 0;
int reverse = 0;
int fold = 0;
int directory = 0;
int key_field = 0;

char *alloc(int n) {
    if (allocbuf + ALLOCSIZE - allocp >= n) {
        allocp += n;
        return allocp - n;
    } else {
        return 0;
    }
}

int mgetline(char *s, int lim) {
    int c, i = 0;
    while (--lim > 0 && (c = getchar()) != EOF && c != '\n')
        s[i++] = c;
    if (c == '\n')
        s[i++] = c;
    s[i] = '\0';
    return i;
}

int readlines(char *lineptr[], int maxlines) {
    int len, nlines = 0;
    char line[MAXLEN];
    char *p;

    while ((len = mgetline(line, MAXLEN)) > 0) {
        if (nlines >= maxlines || (p = alloc(len)) == NULL)
            return -1;
        line[len - 1] = '\0';
        strcpy(p, line);
        lineptr[nlines++] = p;
    }
    return nlines;
}

void writelines(char *lineptr[], int nlines) {
    for (int i = 0; i < nlines; i++)
        printf("%s\n", lineptr[i]);
}

char *get_field(char *line, int field_num) {
    static char field[MAXLEN];
    int count = 0;

    while (*line) {
        while (*line && isspace((unsigned char)*line)) line++;
        if (*line == '\0') break;
        char *start = line;
        while (*line && !isspace((unsigned char)*line)) line++;
        count++;
        if (count == field_num) {
            int len = line - start;
            strncpy(field, start, len);
            field[len] = '\0';
            return field;
        }
    }
    return "";
}

int numcmp(void *a, void *b) {
    char *s1 = *(char **)a;
    char *s2 = *(char **)b;
    if (key_field > 0) {
        s1 = get_field(s1, key_field);
        s2 = get_field(s2, key_field);
    }
    double v1 = atof(s1);
    double v2 = atof(s2);
    if (v1 < v2) return reverse ? 1 : -1;
    if (v1 > v2) return reverse ? -1 : 1;
    return 0;
}

int mstrcmp(void *a, void *b) {
    char *s1 = *(char **)a;
    char *s2 = *(char **)b;
    if (key_field > 0) {
        s1 = get_field(s1, key_field);
        s2 = get_field(s2, key_field);
    }
    for (; *s1 && *s2; s1++, s2++) {
        char c1 = fold ? tolower((unsigned char)*s1) : *s1;
        char c2 = fold ? tolower((unsigned char)*s2) : *s2;
        if (c1 != c2)
            return reverse ? c2 - c1 : c1 - c2;
    }
    return reverse ? *s2 - *s1 : *s1 - *s2;
}

void mqsort(void *v[], int left, int right, int (*comp)(void *, void *)) {
    int i, last;
    void swap(void *v[], int i, int j) {
        void *temp = v[i];
        v[i] = v[j];
        v[j] = temp;
    }
    if (left >= right) return;
    swap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++)
        if ((*comp)(v[i], v[left]) < 0)
            swap(v, ++last, i);
    swap(v, left, last);
    mqsort(v, left, last - 1, comp);
    mqsort(v, last + 1, right, comp);
}

int main(int argc, char *argv[]) {
    int nlines;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0)
            numeric = 1;
        else if (strcmp(argv[i], "-r") == 0)
            reverse = 1;
        else if (strcmp(argv[i], "-f") == 0)
            fold = 1;
        else if (strncmp(argv[i], "-k", 2) == 0)
            key_field = atoi(argv[i] + 2);
    }

    if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
        mqsort((void **)lineptr, 0, nlines - 1, numeric ? numcmp : mstrcmp);
        writelines(lineptr, nlines);
        return 0;
    } else {
        printf("input too big to sort\n");
        return 1;
    }
}
