// find -nx pattern -> print lines that don't match pattern, preceding with line number
#include <stdio.h>
#include <string.h>
#define MAXLINE 1000

int getline(char s[], int lim) {
    int c, i=0;
    while(--lim>0 && (c=getchar()) != EOF && c!='\n')
        s[i++] = c;
    if (c=='\n')
        s[i++] = c;
    s[i] = '\0';
    return i;
}

int main(int argc, char* argv[]) {
    char line[MAXLINE];
    long lineno = 0;
    // except: flag -x or not; number: flag -n or not
    int c, except = 0, number = 0, found = 0;

    // - (in '-nx')
    while(--argc>0 && (*++argv)[0] == '-') {
        // nx (in '-nx')
        while (c = *++argv[0]) {
            switch(c) {
            case 'x':
                except = 1;
                break;
            case 'n':
                number = 1;
                break;
            default:
                printf("find: illegal option %c\n", c);
                argc = 0;
                found = -1;
                break;
            }
        }
    }

    if (argc != 1)
        printf("Usage: find -x -n pattern\n");
    else
        while (getline(line, MAXLINE) > 0) {
            lineno++;
            // print only when (find-substr & not-except) OR (substr-not-found & except) 
            if((strstr(line, *argv) != NULL) != except) {
                if (number)
                    printf("%ld: ", lineno);
                printf("%s", line);
                found++;
            }
        }

    return found;
}