// modify the pattern finding program in chapter5 to take input from a set of named files, or, ir no named files as arguments, from stdin
// gcc exercise7_7.c
// "./a.out dcl_input.txt" or "./a.out"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

FILE* fp = NULL;
int tokentype; // NAME, BRACKETS or PARENS
char token[MAXTOKEN];
char name[MAXTOKEN]; //  name of function e.g. "getx"
char datatype[MAXTOKEN]; // return type of the function declaration
char out[1000];

// buffer for getch and ungetch
#define BUFSIZE 100
char buf[BUFSIZE];
int bufp = 0;
int getch(void) {
    return (bufp>0) ? buf[--bufp] : fgetc(fp); // NOTICE
}
void ungetch(int c) {
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else 
        buf[bufp++] = c;
}

void dcl(void);
void dirdcl(void);

enum {NAME, PARENS, BRACKETS};

int gettoken(void) {
    int c;
    char* p = token;
    while((c=getch()) == ' ' || c=='\t');
    if (c=='(') {
        if ((c=getch()) == ')') {
            strcpy(token, "()");
            return tokentype=PARENS;
        } else {
            ungetch(c);
            return tokentype = '(';
        }
    } else if (c=='[') {
        for(*p++ = c; (*p++ = getch()) != ']';);
        *p = '\0';
        return tokentype = BRACKETS;
    } else if (isalpha(c)) {
        for (*p++=c; isalnum(c=getch());)
            *p++ = c;
        *p = '\0';
        ungetch(c);
        return tokentype=NAME;
    } else if (c==EOF) {
        return EOF;
    } else {
        token[0] = c;
        token[1] = '\0';
        return tokentype=c;
    }
}

void dcl(void) {
    int ns;
    // consuming all the '*'
    for (ns=0; gettoken() == '*';)
        ns++; 
    dirdcl();
    while(ns-- > 0)
        strcat(out, " pointer to");
}

void dirdcl(void) {
    int type;
    if (tokentype == '(') {
        dcl();
        if (tokentype != ')')
            printf("error: missing )\n");
    } else if (tokentype == NAME) {
        strcpy(name, token);
    } else {
        printf("error: expected name of (dcl)\n");
    }
    // solve the [] or ()
    while((type=gettoken()) == PARENS || type==BRACKETS) {
        if (type==PARENS)
            strcat(out, " function returning");
        else {
            strcat(out, " array");
            strcat(out, token); // e.g. [3]
            strcat(out, " of");
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [input_file]\n", argv[0]);
        return 1;
    }
    fp = (argc==2) ? fopen(argv[1], "r") : stdin;
    if (!fp) {
        perror("Cannot open file");
        return 1;
    }

    while (gettoken() != EOF) {
        strcpy(datatype, token); // first input word is "void"/"int"/..
        out[0] = '\0';
        dcl();
        if (tokentype != '\n')
            printf("syntax error\n");
        printf("%s: %s %s\n", name, out, datatype);
    }
    if (fp != stdin) 
        fclose(fp);
    return 0;
}