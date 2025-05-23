// gcc binary_tree.c
// ./a < binary_tree_input.txt
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define MAXWORD 100 // maximum length of a word
#define BUFSIZE 100

struct tnode {
    char* word;
    int count;
    struct tnode* left; 
    struct tnode* right;
};

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
    while (isspace(c=getch())); // consume empty spaces
    if (c!=EOF) *w++ = c;
    // special chars will return immediately
    if (!isalpha(c)) {
        *w = '\0';
        return c;
    }
    for (; --lim>0; w++)
        if (!isalnum(*w = getch())) {
            ungetch(*w);
            break;
        }
    *w = '\0';
    return word[0];
}

// create memory space for a tnode
struct tnode* talloc() {
    return (struct tnode*) malloc(sizeof(struct tnode));
}

// make a duplicate of a string s
char* mstrdup(char* s) {
    char* p;
    p = (char*) malloc(strlen(s)+1); // +1 for '\0'
    if (p != NULL)
        strcpy(p, s);
    return p;
}

// add a word at node 'p' or below
struct tnode* addtree(struct tnode* p, char* w) {
    int cond;
    // new word arrives at this node
    if (p == NULL) {
        p = talloc();
        p->word = mstrdup(w);
        p->count = 1;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0)
        p->count++; // increase if it comes again
    else if (cond < 0)
        p->left = addtree(p->left, w);
    else 
        p->right = addtree(p->right, w);
    
    return p;
}

void treeprint(struct tnode* p) {
    if (p!=NULL) {
        treeprint(p->left);
        printf("%4d %s\n", p->count, p->word);
        treeprint(p->right);
    }
}

int main() {
    struct tnode* root;
    char word[MAXWORD];
    root = NULL;
    while(getword(word, MAXWORD) != EOF)
        if (isalpha(word[0]))
            root = addtree(root, word);
    treeprint(root);
    return 0;
}