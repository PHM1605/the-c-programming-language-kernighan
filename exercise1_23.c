// Write a program to remove all comments from a C program. 
#include <stdio.h>
#include <stdbool.h>
#define MAXLINE 1000

int get_code(char code[]) {
    int i;
    int c;
    while(i<MAXLINE && (c=getchar()) != EOF) {
        code[i++] = c;
    }
    code[i] = '\0';
    return i;
}

void remove_comments(char code[], char clean_code[], int len_orig) {
    int c, k, i=0, j=0;
    while((c=code[i]) != '\0') {
        if (c=='/') {
            // when exceed the last char
            if (i+3 == len_orig) {
                clean_code[j++] = c;
            } else {
                if (code[i+1] == '/') {
                    while((c=code[++i]) != '\0' && c != '\n') {}
                } else if (code[i+1] == '*') {
                    i=i+2;
                    for(int k=i; !(code[k]=='*' && code[k+1]=='/'); k++) {}
                    i=k;
                }
            }
        } else {
            clean_code[j++] = c;
            i++;
        }
        
    }
    clean_code[i] = '\0';
}

int main() {
    char code[MAXLINE];
    char clean_code[MAXLINE];
    int len_orig; // length of original text
    len_orig = get_code(code);
    remove_comments(code, clean_code, len_orig);
    printf("%s", clean_code);
    return 0;
}