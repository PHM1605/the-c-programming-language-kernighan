// Reverse a string
#include <stdio.h>
#define MAXLINE 1000

int get_line(char line[], int limit) {
    int i, c;
    for (i=0; i<limit-1 && (c=getchar())!=EOF && c!='\n'; ++i) {
        line[i] = c;
    }
    if (c=='\n') {
        line[i] = c;
        ++i; 
    }
    line[i] = '\0';
    return i;
}

void reverse_string(char line[], char reverse[]) {
    int i, j;
    while(line[i] != '\0') {
        i++;
    }
    i--; // i at the location before '\0'
    for (j=0; j<(i+1); j++) {
        reverse[j] = line[i]
    }
}

int main() {
    return 0;
}