// keyword counting program: count #occurences of keywords
// gcc word_count_struct.c
// ./a.out < word_count_struct_input.txt
// Note: don't count words in string constants, comments, or preprocessor; handle underscores in names
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD 100
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

#define NKEYS (sizeof keytab / sizeof (struct key))
static char buf[BUFSIZE];
static int bufp = 0;

// get next word or character from input, saving to "word"
// return word[0], or EOF, or 1st special chars
int getword(char* word, int lim) {
    int c;
    int getch(void) {
        return (bufp>0) ? buf[--bufp] : getchar();
    }
    void ungetch(int c) {
        if (bufp>=BUFSIZE)
            printf("ungetch: too many characters\n");
        else {
            buf[bufp++] = c;
        }
    }

    char* w = word;
    while(isspace(c = getch())); // comsume empty spaces
    if (c != EOF) *w++ = c;

    // Skip preprocessor line 
    if (c=='#') {
        while ((c=getch()) != '\n' && c != EOF);
        return getword(word, lim);
    }

    // Skip string literal
    if (c=='"') {
        if (c=='"') {
            while((c=getch()) != '"' && c != EOF);
            return getword(word, lim);
        }
    }

    // Skip comments
    if (c=='/') {
        int next = getch();
        // single-line comment
        if (next == '/') {
            while((c=getch()) != '\n' && c!=EOF);
            return getword(word, lim);
        } 
        // multi-line comment
        else if (next == '*') {
            int prev;
            while((prev = getch()) != EOF) {
                if (prev == '*' && (c=getch()) == '/')
                    break;
            }
            return getword(word, lim);
        } else {
            ungetch(next);
        }
    }

    
    // special chars will return immediately
    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }

    // cover also underscore 
    for (; --lim>0; w++) {
        if ((*w=getch())!='_' && !isalnum(*w)) {
            ungetch(*w);
            break;
        }
    }
    *w = '\0';
    return word[0];
}

// find word in tab[0],...,tab[n-1]
int binsearch(char* word, struct key tab[], int n) {
    int cond;
    int low, high, mid;
    low = 0;
    high = n-1;
    while(low<=high) {
        mid = (low+high) / 2;
        if ((cond = strcmp(word, tab[mid].word)) < 0)
            high = mid-1;
        else if (cond>0)
            low = mid+1;
        else 
            return mid;
    }
    return -1;
}

int main() {
    int n;
    char word[MAXWORD]; // one word
    char test;
    while((test=getword(word, MAXWORD)) != EOF){
        if (isalpha(word[0]))
            if ((n = binsearch(word, keytab, NKEYS)) >= 0)
                keytab[n].count++;
    }    
    printf("STARTING\n");
    //  display the whole table (with >0 occurences) to debug
    for (n=0; n<NKEYS; n++)
        if(keytab[n].count > 0)
            printf("%4d %s\n", keytab[n].count, keytab[n].word);
    return 0;
}