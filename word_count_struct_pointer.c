// count keywords, poiters to structs version
// gcc word_count_struct_pointer.c
// ./a < word_count_struct_input.txt
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define MAXWORD 100 // max length of 1 word
#define BUFSIZE 100
struct key {
    char* word;
    int count;
} keytab[] = {
    "auto", 0,
    "break", 0,
    "case", 0,
    "char", 0,
    "const", 0,
    "continue", 0,
    "default", 0,
    "unsigned", 0,
    "void", 0,
    "volatile", 0,
    "while", 0
};

#define NKEYS (sizeof keytab / sizeof(struct key))
static char buf[BUFSIZE];
static int bufp;

int getword(char* word, int lim) {
    int c;
    int getch(void) {
        return (bufp>0) ? buf[--bufp] : getchar();
    }
    void ungetch(int c) {
        if (bufp>=BUFSIZE)
            printf("ungetch: too many characters\n");
        else 
            buf[bufp++] = c;
    }
    char* w = word;
    while(isspace(c=getch())); // consume empty spaces
    if (c!=EOF) *w++ = c;
    // special chars will return immediately
    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }
    for (;--lim>0; w++) {
        if (!isalnum(*w = getch())) {
            ungetch(*w);
            break;
        }
    }
    *w = '\0';
    return word[0];
}

// find word in tab[0],..,tab[n-1]
struct key* binsearch(char* word, struct key* tab, int n) {
    int cond;
    struct key* low = &tab[0];
    struct key* high = &tab[n];
    struct key* mid;
    while(low<high) {
        mid = low + (high-low)/2;
        if((cond = strcmp(word, mid->word)) < 0)
            high = mid;
        else if (cond>0)
            low = mid+1;
        else 
            return mid;
    }
    return NULL; // find nothing
}

int main() {
    char word[MAXWORD];
    struct key* p;
    while(getword(word, MAXWORD) != EOF)
        if (isalpha(word[0]))
            if ((p = binsearch(word, keytab, NKEYS)) != NULL)
                p->count++;
    // display search result table
    for (p=keytab; p<keytab+NKEYS; p++)
        if (p->count > 0)
            printf("%4d %s\n", p->count, p->word);
    return 0;
}