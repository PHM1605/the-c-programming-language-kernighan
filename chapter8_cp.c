#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>

#define PERMS 0666 // RW (110) permission for owner, group, others

void error(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "error: ");
    vfprintf(stderr, fmt, args); // vprintf: 'printf' with 'va_list' argument already started
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}

int main(int argc, char* argv[]) {
    int f1, f2; // files descriptors
    int n; // read how many bytes from file
    char buf[BUFSIZ];

    if (argc != 3)
        error("Usage: cp from to");
    if ((f1 = open(argv[1], O_RDONLY, 0)) == -1)
        error("cp: can't open %s", argv[1]);
    if ((f2 = creat(argv[2], PERMS)) == -1)
        error("cp: can't create %s, mode %03o", argv[2], PERMS);
    while((n = read(f1, buf, BUFSIZ)) > 0) {
        if (write(f2, buf, n) != n)
            error("cp: write error on file %s", argv[2]);
    }
    
    return 0;
}