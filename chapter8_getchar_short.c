#include <unistd.h>
#include <stdio.h>

int mgetchar() {
    char c;
    return (read(0, &c, 1) == 1) ? (unsigned char)c : EOF;
}

int main(){
    int c;
    while((c=mgetchar()) != EOF) {
        putchar(c);
    }
    return 0;
}