// show the list of words in decreasing frequency
// gcc exercise6_4.c
// ./a < exercise6_4_input.txt
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> // for malloc 

#define MAXWORD 100 
#define MAXNODES 1000

struct tnode {
    char* word;
    int count;
    struct tnode* left;
    struct tnode* right;
};

struct tnode* nodes[MAXNODES];
int node_count = 0;

int getword(FILE* fp, char* word, int lim) {
    int c, d; // c is to getchar(), d is for mini-buffering
    char* w = word;
    while(isspace(c=fgetc(fp)));
    if (c==EOF) return EOF;
    if (!isalpha(c)) {
        *w++ = c;
        *w = '\0';
        return c;
    }

    *w++ = c;
    while(--lim > 0) {
        c = fgetc(fp);
        if (!isalnum(c)) {
            ungetc(c, fp);
            break;
        }
        *w++ = c;
    }
    *w = '\0';
    return word[0];
}

// add a word at node 'p' or below, sorting based on #occurences
struct tnode* addtree(struct tnode* p, char* w) {
    int cond;
    if (p==NULL) {
        p = (struct tnode*) malloc(sizeof(struct tnode));
        p->word = strdup(w);
        p->count = 1;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0)
        p->count++;
    else if (cond<0)
        p->left = addtree(p->left, w);
    else 
        p->right = addtree(p->right, w);
    return p;
}

void collect_nodes(struct tnode* p) {
    if (p!=NULL) {
        collect_nodes(p->left);
        if (node_count < MAXNODES)
            nodes[node_count++] = p;
        collect_nodes(p->right);
    }
}

// comparing function (for qsort)
int compare_nodes(const void* a, const void* b) {
    struct tnode* n1 = *(struct tnode**) a;
    struct tnode* n2 = *(struct tnode**) b;
    return n2->count - n1->count;
}

void print_by_frequency() {
    collect_nodes(nodes[0]);
    // qsort: array name, nitems, size of each element, sorting function
    qsort(nodes, node_count, sizeof(struct tnode*), compare_nodes);
    for (int i =0; i<node_count; i++) {
        printf("%4d %s\n", nodes[i]->count, nodes[i]->word);
    }
}

int main() {
    struct tnode* root = NULL;
    char word[MAXWORD];
    while(getword(stdin, word, MAXWORD) != EOF) {
        if (isalpha(word[0])) 
            root = addtree(root, word);
    }
    nodes[0] = root;
    print_by_frequency();
    return 0;
}