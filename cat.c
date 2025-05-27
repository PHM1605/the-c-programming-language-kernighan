// write a mini-cat program, concatenating contents of file1, file2....to screen

#include <stdio.h>

main(int argc, char* argv[]) {
    FILE* fp;
    char* prog = argv[0]; // program name, for displaying error

    void filecopy(FILE* ifp, FILE* ofp) {
        int c;
        while((c = getc(ifp)) != EOF) 
            putc(c, ofp);
    }
    // if there are command line arguments -> those are file names
    if (argc == 1)
        filecopy(stdin, stdout);
    else
        // if there is none, stdin is processed
        while(--argc > 0) {
            if ((fp = fopen(*++argv, "r")) == NULL) {
                fprintf(stderr, "%s: can't open %s\n", prog, *argv);
                exit(1); // cancal program execution when called (including fclose on opened files)
            } else {
                filecopy(fp, stdout);
                fclose(fp);
            }
        }
    
    // e.g. when disk is full 
    if (ferror(stdout)) {
        fprintf(stderr, "%s: error writing stdout\n", prog);
        exit(2);
    }
    exit(0);
}