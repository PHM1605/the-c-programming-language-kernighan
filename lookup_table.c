
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>

#define HASHSIZE 101 // #elements in pointer table
#define MAXWORD 100

// table entry
struct nlist {
    struct nlist* next; // next table entry
    char* name; // name (to be replaced later)
    char* defn; // replacement text
};

static struct nlist* hashtab[HASHSIZE];

// encrypting a string to positive integer
unsigned hash(char* s) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++) 
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

// check if that string exists or nor (return address if yes)
struct nlist* lookup(char* s) {
    struct nlist* np;
    for (np=hashtab[hash(s)]; np!=NULL; np=np->next) 
        if (strcmp(s, np->name) == 0)
            return np; // found
    return NULL;
}

// put (name, defn) in the table (replace if exists)
// install a cell to the 1st position of hashtab[hashval] linked-list 
struct nlist* install(char* name, char* defn) {
    struct nlist* np;
    unsigned hashval;
    // if 'name' not found in table
    if ((np = lookup(name)) == NULL) {
        np = (struct nlist* ) malloc(sizeof(*np));
        // if memory allocation failed
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        // after memory allocation
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } 
    // 'name' already in the table
    else 
        free((void*) np->defn); // free previous defn
    
    if ((np->defn = strdup(defn)) == NULL)
        return NULL;
    return np;
}

int ndef(char *name) {
    unsigned hashval = hash(name);
    struct nlist* curr = hashtab[hashval];
    struct nlist* prev = NULL;
    while(curr != NULL) {
        // found the entry to remove
        if (strcmp(curr->name, name) == 0) {
            if (prev == NULL)
                hashtab[hashval] = curr->next;
            else  // make 'prev' skipping over to the next entry
                prev->next = curr->next;
            free(curr->name);
            free(curr->defn);
            free(curr);
            return 1; // success
        }
        // move 'curr' and 'prev' to the next entries
        prev = curr;
        curr = curr->next;
    }
    return 0; // not found
}

#define BUFSIZE 100
char buf[BUFSIZE];
int bufp = 0;
int getch() {
    return (bufp>0) ? buf[--bufp] : getchar();
}
void ungetch(int c) {
    if (bufp>=BUFSIZE)
        printf("ungetch: too many characters\n");
    else 
        buf[bufp++] = c;
}

int getword(char* word, int lim) {
    int c;
    char* w = word;
    while (isspace(c=getch()) && c!='\n');

    if (c!=EOF) *w++ = c;

    if (!isalnum(c) && c!='.') {
        *w = '\0';
        return c;
    }

    for (; --lim>0; w++) {
        int next = getch();
        if (!isalnum(next) && next != '.') {
            ungetch(next);
            break;
        }
        *w = next;
    }
    *w = '\0';
    return word[0];
}

int main() {
    char word[MAXWORD];
    struct nlist* np;
    // print definition of found; otherwise print the original word
    while(getword(word, MAXWORD) != EOF) {
        if (strcmp(word, "#") == 0) {
            getword(word, MAXWORD);
            if (strcmp(word, "define") == 0) {
                char name[MAXWORD], defn[MAXWORD];
                getword(name, MAXWORD);
                getword(defn, MAXWORD);
                install(name, defn);
            }
        } else if ((np = lookup(word)) != NULL) {
            printf("%s", np->defn);
        } else {
            printf("%s", word);
        }
    }
    return 0;
}