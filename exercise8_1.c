// Write a 'cat' program using 'read','write','open','close'
// gcc exercise8_1.c
// ./a.out exercise7_8_input1.txt exercise7_8_input2.txt
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include<fcntl.h>

void filecopy(int in_fd, int out_fd) {
    char buf[BUFSIZ];
    ssize_t n;
    while((n=read(in_fd, buf, BUFSIZ)) > 0) {
        if (write(out_fd, buf, n) != n) {
            fprintf(stderr, "cat - write error\n");
            exit(3);
        }
    }
    if (n < 0) {
        fprintf(stderr, "cat: read error\n");
        exit(4);
    }
}

int main(int argc, char* argv[]) {
    int fd; // file descriptor
    // if there is no argument, copy directly input to output
    char* prog = argv[0];

    if (argc == 1) {
        filecopy(STDIN_FILENO, STDOUT_FILENO);
    } else {
        while(--argc > 0) {
            if ((fd = open(*++argv, O_RDONLY)) < 0) {
                fprintf(stderr, "%s: can't open %s\n", prog, *argv);
                exit(1);
            } else {
                filecopy(fd, STDOUT_FILENO);
                close(fd);
            }
        }
    }
    // check for write errors to stdout
    if (write(STDOUT_FILENO, "", 0) == -1) {
        fprintf(stderr, "%s: error writing to stdout\n", prog);
        exit(2);
    }
    return 0;
}