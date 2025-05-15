// Write a program to remove all comments from a C program. 
#include <stdio.h>
#include <stdbool.h>
#define MAXLINE 1000

int get_code(char code[]) {
    int i=0;
    int c;
    while(i<MAXLINE && (c=getchar()) != EOF) {
        code[i++] = c;
    }
    code[i] = '\0';
    return i;
}

void remove_comments(char code[], char clean_code[]) {
    int c, k, i=0, j=0;
    bool inside_comment_oneline = false;
    bool inside_comment_multiline = false;
    while((c=code[i]) != '\0') {
        if (c=='/') {
            if (code[i+1] == '/') {
                inside_comment_oneline = true;
                i++;
            } else if (code[i+1] == '*') {
                inside_comment_multiline = true;
                i++;
            }
        } else if (c=='\n' && inside_comment_oneline ) {
            inside_comment_oneline = false;
        } else if (c=='*' && code[i+1]=='/' && inside_comment_multiline) {
            inside_comment_multiline = false;
            i++;
        } else {
            if ((!inside_comment_oneline) && (!inside_comment_multiline)) {
                clean_code[j++] = c;
            }            
        }     
        i++;   
    }
    clean_code[j] = '\0';
}

int main() {
    char code[MAXLINE];
    char clean_code[MAXLINE];
    get_code(code);
    remove_comments(code, clean_code);
    printf("%s", clean_code);
    return 0;
}