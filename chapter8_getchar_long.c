#include <stdio.h>
#include <unistd.h>

int mgetchar(void) {
    static char buf[BUFSIZ];
    static char *bufp = buf;
    static int n = 0; // size of chars loaded into buffer

    // if buffer is empty-> read more
    if (n==0) {
        n = read(0, buf, sizeof buf);
        bufp = buf;
    }
    return (--n>=0) ? (unsigned char)*bufp++ : EOF;
}

int main() {
    int c;
    while((c=mgetchar()) !=EOF) {
        putchar(c);
    }
    return 0;
}