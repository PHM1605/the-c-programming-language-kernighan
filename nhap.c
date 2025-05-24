#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXWORD 100
#define DEFAULT_PREFIX_LEN 6

struct tnode {
    char *word;
    struct tnode *left;
    struct tnode *right;
};

struct group {
    char prefix[MAXWORD];
    struct tnode *words;
    struct group *next;
};

int prefix_len = DEFAULT_PREFIX_LEN;

int getword(FILE *fp, char *word, int lim);
int is_variable_start(int c);
struct group *addgroup(struct group *gp, char *word);
struct tnode *addtree(struct tnode *p, char *w);
void treeprint(struct tnode *p);
void printgroups(struct group *gp);

// Entry point
int main(int argc, char *argv[]) {
    if (argc == 2) {
        prefix_len = atoi(argv[1]);
        if (prefix_len <= 0) {
            fprintf(stderr, "Invalid prefix length.\n");
            return 1;
        }
    }

    struct group *grouplist = NULL;
    char word[MAXWORD];
    while (getword(stdin, word, MAXWORD) != EOF) {
        if (isalpha(word[0]) || word[0] == '_') {
            grouplist = addgroup(grouplist, word);
        }
    }

    printgroups(grouplist);
    return 0;
}

// Skip string literals, comments, etc., and extract next word
int getword(FILE *fp, char *word, int lim) {
    int c, d;
    char *w = word;

    while (isspace(c = getc(fp)))
        ;

    if (c == EOF)
        return EOF;

    if (c == '"') {
        // skip string
        while ((d = getc(fp)) != EOF && d != '"') {
            if (d == '\\') getc(fp); // skip escape
        }
        return getword(fp, word, lim);
    }

    if (c == '/') {
        if ((d = getc(fp)) == '/') {
            while ((c = getc(fp)) != EOF && c != '\n');
            return getword(fp, word, lim);
        } else if (d == '*') {
            while ((c = getc(fp)) != EOF) {
                if (c == '*' && (d = getc(fp)) == '/') break;
            }
            return getword(fp, word, lim);
        } else {
            ungetc(d, fp);
        }
    }

    if (!isalpha(c) && c != '_') {
        word[0] = c;
        word[1] = '\0';
        return c;
    }

    *w++ = c;
    while (--lim > 0) {
        c = getc(fp);
        if (!isalnum(c) && c != '_') {
            ungetc(c, fp);
            break;
        }
        *w++ = c;
    }
    *w = '\0';
    return word[0];
}

// Check for variable identifier
int is_variable_start(int c) {
    return isalpha(c) || c == '_';
}

// Add variable name to group
// the newly added group (if currently not exists) will be the new root group*
struct group *addgroup(struct group *gp, char *word) {
    char prefix[MAXWORD];
    strncpy(prefix, word, prefix_len);
    prefix[prefix_len] = '\0';

    struct group *g;
    for (g = gp; g != NULL; g = g->next) {
        if (strcmp(g->prefix, prefix) == 0)
            break;
    }

    if (g == NULL) {
        g = (struct group *)malloc(sizeof(struct group));
        strcpy(g->prefix, prefix);
        g->words = NULL;
        g->next = gp;
        gp = g;
    }

    g->words = addtree(g->words, word);
    return gp;
}

// Add word to binary search tree
struct tnode *addtree(struct tnode *p, char *w) {
    int cond;
    if (p == NULL) {
        p = (struct tnode *)malloc(sizeof(struct tnode));
        p->word = strdup(w);
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) < 0)
        p->left = addtree(p->left, w);
    else if (cond > 0)
        p->right = addtree(p->right, w);
    return p;
}

// Print tree (sorted)
void treeprint(struct tnode *p) {
    if (p != NULL) {
        treeprint(p->left);
        printf("  %s\n", p->word);
        treeprint(p->right);
    }
}

// Print all groups with more than 1 unique word
void printgroups(struct group *gp) {
    for (; gp != NULL; gp = gp->next) {
        // Check if more than one word exists
        int count = 0;
        void count_nodes(struct tnode *p) {
            if (!p) return;
            count_nodes(p->left);
            count++;
            count_nodes(p->right);
        }
        count = 0;
        count_nodes(gp->words);
        if (count > 1) {
            printf("\nGroup: %s\n", gp->prefix);
            treeprint(gp->words);
        }
    }
}