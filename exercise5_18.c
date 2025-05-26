// make dcl recover from input errors 
// e.g. char (*(*getx[3])())[5] -> "array[3] of pointer to function getx returning pointer to array[5] of char" 
// usage: gcc dcl.c
// ./a.out < dcl_input.txt
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// buffer for getch() and ungetch()
#define BUFSIZE 100
char buf[BUFSIZE];
int bufp = 0;

#define MAXTOKEN 100

void dcl(void);
void dirdcl(void);

enum {NAME, PARENS, BRACKETS};

int tokentype; // type of last token
char token[MAXTOKEN]; // last token value
char name[MAXTOKEN]; // name of function e.g. "getx"
char datatype[MAXTOKEN]; // name of datatype at the beginning e.g. "void"/"int"..
char out[1000]; // output sentence from the declaration

// token is a function-name, (), or []
int gettoken() {
    int c;
    int getch() {
        return (bufp>0) ? buf[--bufp] : getchar();
    }
    void ungetch(int c) {
        if (bufp>=BUFSIZE) 
            printf("ungetch: too many characters\n");
        else
            buf[bufp++] = c;
    }
    char* p = token;

    // skipping white spaces
    while((c=getch()) == ' ' || c=='\t');
    if (c == '(') {
        if ((c=getch()) == ')') {
            strcpy(token, "()");
            return tokentype = PARENS;
        } else {
            ungetch(c);
            return tokentype = '(';
        }
    } else if (c=='[') {
        for (*p++ = c; (*p++=getch())!=']';);
        *p = '\0';
        return tokentype = BRACKETS;
    } else if (isalpha(c)) {
        for (*p++=c; isalnum(c=getch());)
            *p++ = c;
        *p = '\0';
        ungetch(c);
        return tokentype = NAME;
    } else 
        return tokentype=c;
}

void dcl(void) {
    int ns;
    // count # of * 
    for(ns = 0; gettoken()=='*'; ) ns++;
    dirdcl();
    
    while(ns-->0)
        strcat(out, " pointer to");
}

void dirdcl(void) {
    int type;
    // (dcl)
    if (tokentype=='(') {
        dcl();
        if (tokentype != ')') {
            printf("error: missing )\n");
            // discard input until the end
            while(tokentype != '\n' && tokentype !=EOF) {
                gettoken();
                
            }
            out[0]= '\0';
            return;
        }
            
    } 
    // variable name
    else if (tokentype==NAME) 
        strcpy(name, token);
    else {
        printf("error: expected name or (dcl)\n");
    }
        

    while ((type=gettoken())==PARENS || type==BRACKETS)
        if (type==PARENS)
            strcat(out, " function returning");
        else {
            strcat(out, " array");
            strcat(out, token); // [3]
            strcat(out, " of");
        }
}



int main() {
    while(gettoken() != EOF) {
        strcpy(datatype, token); // first input word is "void"/"int"/..
        out[0] = '\0';
        dcl(); // parse the rest of the line
        if (tokentype != '\n') {
            printf("syntax error\n");
        }
        printf("%s: %s %s\n", name, out, datatype);
    }
    return 0;
}