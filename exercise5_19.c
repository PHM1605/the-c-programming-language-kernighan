// modify dcl.c so that it does not add redundant parenthesis to declaration
// x * char => char*x instead of char(*x)
// x * () char => char (*x)()
// ./a < exercise5_19_input.txt
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100
#define BUFSIZE 100

char buf[BUFSIZE];
int bufp = 0;
char token[MAXTOKEN]; // last token value
int tokentype;
char out[MAXTOKEN]; // output sentence
enum {NAME, PARENS, BRACKETS};

// token is a function-name, () or [size]
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
    while((c=getch())==' ' || c=='\t');
    if (c=='(') {
        if ((c=getch()) == ')') {
            strcpy(token, "()");
            return tokentype = PARENS;
        } else {
            ungetch(c);
            return tokentype = '(';
        }
    } else if (c=='[') {
        for (*p++ = c; (*p++=getch()) != ']';);
        *p = '\0';
        return tokentype = BRACKETS;
    } else if (isalpha(c)) {
        for (*p++=c; isalnum(c=getch());)
            *p++ = c;
        *p = '\0';
        ungetch(c);
        return tokentype = NAME;
    } else 
        return tokentype = c;
}

int main() {
    int type;
    char temp[1000];
    int prevtoken;

    while(gettoken() != EOF) {
        prevtoken = NAME; // first token is always NAME
        strcpy(out, token);
        printf("%s", out); // getx
        while((type = gettoken()) != '\n')
            if (type==PARENS || type==BRACKETS) {
                if (prevtoken == '*') {
                    sprintf(temp, "(%s)", out);
                    strcpy(out, temp);
                }
                strcat(out, token);
                prevtoken = type;
            }
                
            else if (type=='*') {
                prevtoken = '*';
                sprintf(temp, "*%s", out);
                strcpy(out, temp);
            } 
            // type at 1st position
            else if (type==NAME) {
                sprintf(temp, "%s %s", token, out);
                strcpy(out, temp);
            } else 
                printf("invalid input at %s\n", token);
        printf(" -> %s\n", out); // -> char (*(*x())[])()
    }
    return 0;
}