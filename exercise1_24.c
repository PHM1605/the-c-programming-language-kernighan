// Write a program to check rudimentary syntax error (unmatched parentheses, brackets, braces, single quotes, double quotes, escape sequences, comments)
// usage: ./exercise1_24 hello.c
// we'll change hello.c program to check
#include <stdio.h>
#include <stdlib.h>

#define MAXSTACK 1024

void check_syntax(FILE* fp) {
    int ch;
    while ((ch=fgetc(fp)) != EOF) {
        putchar(ch);
    }
}

int main(int argc, char* argv[]) {
    if (argc<2) {
        printf("Usage: %s <source_file.c>\n", argv[0]);
        return 1;
    }
    FILE* fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }
    check_syntax(fp);
    fclose(fp);
    return 0;
}

