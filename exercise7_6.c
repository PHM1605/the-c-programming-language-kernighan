// write a program to compare 2 files, printing the 1st line when they differ
#include <stdio.h>
#include <string.h>
#define MAXLINE 1000

// get (at most n chars) a line from input-file
char* mfgets(char* s, int n, FILE* iop) {
    register int c;
    register char* cs;

    cs = s;
    while(--n>0 && (c=getc(iop)) != EOF) 
        if ((*cs++ = c) == '\n') break;
    *cs = '\0';
    return (c==EOF && cs==s) ? NULL : s;
}

int mfputs(char* s, FILE* iop) {
    int c;
    while(c = *s++)
        putc(c, iop);
    return ferror(iop) ? EOF : 0;
}

// int mgetline(char* line, int max) {
//     if (mfgets(line, max, stdin) == NULL)
//         return 0;
//     else 
//         return strlen(line);
// }

int main(int argc, char* argv[]) {
    if (argc!=3){
        fprintf(stderr, "Usage: %s file1 file2\n", argv[0]);
        return 1;
    }

    FILE* fp1 = fopen(argv[1], "r");
    FILE* fp2 = fopen(argv[2], "r");
    if (!fp1 || !fp2) {
        perror("Error opening file");
        return 1;
    }
    char line1[MAXLINE], line2[MAXLINE]; // each line from file1 and file2
    int lineno = 1;
    while(1) {
        char* l1 = mfgets(line1, MAXLINE, fp1);
        char* l2 = mfgets(line2, MAXLINE, fp2);
        // this line is only called when all sentences prior have been compared
        if(l1==NULL && l2==NULL) {
            printf("Files are identical.\n");
            break;
        }
        if(l1==NULL || l2==NULL || strcmp(line1, line2)!=0) {
            printf("Files differ at line %d\n", lineno);
            printf("File1: %s", l1 ? line1 : "(EOF)\n");
            printf("File2: %s", l2 ? line2 : "(EOF)\n");
            break;
        }
        lineno++;
    }
    fclose(fp1);
    fclose(fp2);
    return 0;
}