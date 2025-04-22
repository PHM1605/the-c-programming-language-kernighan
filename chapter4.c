// Find all lines matching a pattern
/* Type this:
Ah Love! could you and I with Fate conspire
To grasp this sorry Scheme of Things entire,
Would not we shatter it to bits -- and then
Re-mould it nearer to the Heart's Desire!
*/
// compile: 
// gcc -c chapter4.c getline.c strindex.c => will generate chapter4.o, getline.o and strindex.o
//  gcc -o aa chapter4.o getline.o strindex.o => will generate aa.exe (Windows)
// => ./aa to run
#include <stdio.h>
#define MAXLINE 1000

int getline(char line[], int max); // get line into 'line', return length
int strindex(char source[], char searchfor[]); // return index of searchfor in source, -1 if none

char pattern[] = "ould";

// find all lines matching pattern
int main() {
    char line[MAXLINE];
    int found = 0;
    while(getline(line, MAXLINE) > 0) {
        if (strindex(line, pattern) >= 0) {
            printf("%s", line);
            found++;
        }
    }
    return found;
}

