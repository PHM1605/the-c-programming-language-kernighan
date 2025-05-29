// show also the lines each word happens
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> // for malloc

#define MAXWORD 100
#define MAXLINES 100

const char* noise_words[] = {
    "the", "and", "a", "an", "of", "to", "in", "on", "for", "with", "at", "by", "from", "up", "about", "into", "over", "after", NULL
}; // NULL at the end is to stop counting #noise_words

struct tnode {
    char* word;
    int lines[MAXLINES]; // which line that word happens
    int line_count;
    struct tnode* left; 
    struct tnode* right;
};

// check if that word is in the list of noise words -> skip
int is_noise(const char* w) {
    for(int i=0; noise_words[i]; i++) {
        if (strcmp(w, noise_words[i]) == 0)
            return 1;
    }
    return 0;
}

struct tnode* addtree(struct tnode* p, char* w, int line) {
    int cond;
    if (p == NULL) {
        p = (struct tnode*) malloc(sizeof(struct tnode));
        p->word = strdup(w);
        p->line_count = 1;
        p->lines[0] = line;
        p->left = p->right = NULL;
    } else if ((cond = strcmp(w, p->word)) == 0) {
        if (p->lines[p->line_count-1] != line && p->line_count<MAXLINES)
            p->lines[p->line_count++] = line;
    } else if (cond<0) {
        p->left = addtree(p->left, w, line);
    } else {
        p->right = addtree(p->right, w, line);
    }
    return p;
}

void treeprint(struct tnode* p) {
    if (p!=NULL) {
        treeprint(p->left);
        printf("%s: ", p->word);
        for (int i=0; i<p->line_count; i++) {
            printf("%d%s", p->lines[i], (i<p->line_count-1)?", ":"");
        }
        printf("\n");
        treeprint(p->right);
    }
}

int getword(char* word, int limit, FILE* fp) {
    int c;
    char* w = word;
    while (isspace(c=fgetc(fp))) {
        if (c=='\n') {
            *w++ = '\n';
            *w = '\0';
            return '\n';
        }
    }
    if (c==EOF) return EOF;

    if (!isalpha(c)) {
        *w++ = c;
        *w = '\0';
        return c;
    }

    *w++ = c;
    while(--limit > 0) {
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

int main() {
    struct tnode* root = NULL;
    char word[MAXWORD];
    int line = 1;

    while(getword(word, MAXWORD, stdin) != EOF) {
        if (isalpha(word[0]) && !is_noise(word)) {
            root = addtree(root, word, line);
        } else if (word[0] == '\n') {
            line++;
        }
    }
    treeprint(root);
    return 0;
}