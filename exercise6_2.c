// ./a 6 < exercise6_2_input.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXWORD 100
int prefix_len; // how many of the first chars are the same for each word group

struct tnode {
    char* word;
    struct tnode* left; 
    struct tnode* right;
};

struct group {
    char prefix[MAXWORD];
    struct tnode* words;
    struct group* next;
};

int getword(FILE* fp, char* word, int lim) {
    int c, d; // c is to getchar(), d is a mini-buffer
    char* w = word;
    while (isspace(c = getc(fp)));
    if (c==EOF)
        return EOF;
    
    // skip string
    if (c== '"') {
        while((d=getc(fp)) != EOF && d!='"') {
            if (d=='\\') getc(fp); // skip escape chars \" if exist in string
         }
        return getword(fp, word, lim);
    }

    // skip comments
    if (c=='/') {
        // line comment
        if ((d=getc(fp)) == '/') {
            while ((c=getc(fp)) != EOF && c!='\n');
            return getword(fp, word, lim);
        } else if (d=='*') {
            while((c=getc(fp)) != EOF) {
                if (c == '*' && (d = getc(fp)) == '/') break;
            }
            return getword(fp, word, lim);
        } else {
            ungetc(d, fp);
        }
    }

    // return back special characters
    if (!isalpha(c) && c != '_') {
        word[0] = c;
        word[1] = '\0';
        return c;
    }

    // start getting words
    *w++ = c;
    while(--lim>0) {
        c = getc(fp);
        if (!isalnum(c) && c!='_') {
            ungetc(c, fp);
            break;
        }
        *w++ = c;
    }
    *w = '\0';
    return word[0];
}

// Add word to 'words' tree
struct tnode* addtree(struct tnode* p, char* w) {
    int cond;
    if (p==NULL) {
        p = (struct tnode*) malloc(sizeof( struct tnode));
        p->word = strdup(w);
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) < 0)
        p->left = addtree(p->left, w);
    else if (cond > 0)
        p->right = addtree(p->right, w);
    return p;
}

struct group* addgroup(struct group* gp, char* word) {
    char prefix[MAXWORD];
    strncpy(prefix, word, prefix_len);
    prefix[prefix_len] = '\0';

    // find the suitable group for that word (create if not exists)
    struct group* g;
    for (g = gp; g!=NULL; g=g->next) {
        if (strcmp(g->prefix, prefix) == 0)
            break;
    }

    if (g == NULL) {
        g = (struct group*) malloc(sizeof(struct group));
        strcpy(g->prefix, prefix);
        g->words = NULL;
        g->next = gp;
        gp = g;
    }
    g->words = addtree(g->words, word);
    return gp;
}

void treeprint(struct tnode* p) {
    if (p != NULL) {
        treeprint(p->left);
        printf(" %s\n", p->word);
        treeprint(p->right);
    }
}

int count_nodes(struct tnode* p) {
    if (!p) return 0;
    return 1 + count_nodes(p->left) + count_nodes(p->right);
}

void printgroups(struct group* gp) {
    for (; gp != NULL; gp = gp->next) {
        int count = count_nodes(gp->words);
        if (count > 1) {
            printf("\nGroup: %s\n", gp->prefix);
            treeprint(gp->words);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        prefix_len = atoi(argv[1]);
        if (prefix_len <= 0) {
            fprintf(stderr, "Invalid prefix length.\n");
            return 1;
        }
    }

    struct group* grouplist = NULL;
    char word[MAXWORD];
    while (getword(stdin, word, MAXWORD) != EOF) {
        if (isalpha(word[0]) || word[0] == '_') {
            grouplist = addgroup(grouplist, word);
        }
    }

    printgroups(grouplist);

    return 0;
}