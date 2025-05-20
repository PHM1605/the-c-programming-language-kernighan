// entab: convert spaces = tabs+spaces
// ./entab 4 10 16 < input.txt
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXSTOPS 100 // maximum number of tabs inserted
#define DEFAULT_TAB 8 // tab size

int tabstops[MAXSTOPS];
int nstops = 0;

// col: each entering char is adding 1 column
int is_tabstop(int col) {
    // if no argument of tabstop, use default
    if (nstops == 0)
        return col%DEFAULT_TAB;
    for (int i=0; i<nstops; ++i) {
        if (tabstops[i] == col)
            return 1;
    }
    return 0;
}

// int main(int argc, char* argv[]) {
//     // load arguments into tabstops array
//     for (int i=1; i<argc && nstops<MAXSTOPS; ++i) {
//         if (isdigit(argv[i][0])) {
//             tabstops[nstops++] = atoi(argv[i]);
//         }
//     }

//     int c, col=0, space_count=0;
//     while((c=getchar()) != EOF) {
//         if (c==' ') {
//             ++space_count;
//             if (is_tabstop(col)) {
//                 putchar('\t'); // can put 't' here for brevity
//                 space_count = 0;
//             }
//             ++col;
//         } else {
//             while (space_count-- > 0) 
//                 putchar(' ');
//             putchar(c);
//             if (c=='\n')
//                 col = 0;
//             else
//                 ++col;
//         }
//     }
//     return 0;
// }

// detab: convert tab+spaces => spaces
int next_tabstop(int col) {
    if (nstops == 0) 
        return col + (DEFAULT_TAB - col%DEFAULT_TAB);
    
    for (int i=0; i<nstops; ++i) {
        if (tabstops[i] > col)
            return tabstops[i];
    }
    return col+1;
}

int main(int argc, char* argv[]) {
    // Load tabstops from command-line arguments
    for (int i=1; i<argc && nstops<MAXSTOPS; ++i) {
        if (isdigit(argv[i][0])) 
            tabstops[nstops++] = atoi(argv[i]);
    }

    int c, col = 0;
    while((c=getchar()) != EOF) {
        if (c=='\t') {
            int next = next_tabstop(col);
            while(col<next) {
                putchar(' ');
                ++col;
            }
        } else {
            putchar(c);
            if (c=='\n')
                col = 0;
            else 
                ++col;
        }
    }
    return 0;
}