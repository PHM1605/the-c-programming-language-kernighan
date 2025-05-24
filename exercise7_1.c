// write a program that converts upper case to lower or lower case to upper, depending on the name it is invoked with, as found in argv[0]
// gcc exercise7_1.c -o lower => ./lower
// gcc exercise7_1.c -o upper => ./upper

#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char* argv[]) {
    int c;
    // strstr: if "upper" inside the string argv[0] only
    // strcmp: two strings must be strictly equal
    if (strstr(argv[0], "upper") != NULL) {
        while((c=getchar()) != EOF) {
            putchar(toupper(c));
        }
    } else if (strstr(argv[0], "lower") != NULL) {
        while((c=getchar()) != EOF) {
            putchar(tolower(c));
        }
    } else { // fallback
        while((c=getchar())!=EOF) {
            putchar(c);
        }
    }
    return 0;
}