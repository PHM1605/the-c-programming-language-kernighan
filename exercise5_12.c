// entab: spaces->tab; detab: tab->spaces
// 'entab -m +n' means tab stops every n-columns, starting at column m
// gcc exercise5_12.c -o entab
// ./entab -10 +5 < input.txt 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP 100
#define DEFAULT_TAB 8
char op[MAXOP];
int op_loc = 0;
#define MAXSTOPS 100 // maximum #tabs inserted
int tabstops[MAXSTOPS];

// start_loc is pointing to '-abc'; writing 'abc' to stack 'op'
void get_arg(char* start_loc) {
    op_loc = 0;
    while(*(++start_loc) != '\0') {
        op[op_loc++] = *start_loc;
    }
    op[op_loc] = '\0';
}


int is_tabstop(int col) {
    for (int i=0; i<MAXSTOPS; ++i) {
        if (tabstops[i] == col)
            return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    int start_col=-1, col_step=-1;
    for(int i=1; i<argc; i++) {
        // -m means starting at column m
        if (argv[i][0] == '-') {
            get_arg(*(argv+i));
            start_col = atoi(op);
        // +n means next column is (m+n),(m+2n),...
        } else if (argv[i][0] == '+') {
            get_arg(*(argv+i));
            col_step = atoi(op);
        } else {
            printf("ERROR: command format wrong!\n");
        }
    }

    // init the loc table
    if (start_col == -1 || col_step == -1) {
        start_col = DEFAULT_TAB;
        col_step = DEFAULT_TAB;
    }        
    for (int i=0; i<MAXSTOPS; i++) {
        tabstops[i] = start_col + i*col_step;
    }
    
    int c; // read char from input file
    int col = 0; // column indexing
    int space_count = 0; // space counter
    while((c=getchar()) != EOF) {
        if (c==' ') {
            ++space_count;
            if (is_tabstop(col)) {
                putchar('t');
                space_count = 0;
            }
            ++col;
        } else {
            while(space_count > 0) {
                putchar(' ');
                space_count--;
            }                
            putchar(c);
            if (c=='\n')
                col = 0;
            else 
                ++col;
        }
    }
    return 0;
}

// // detab: convert tab->spaces
// // gcc exercise5_12.c -o detab
// // ./detab -10 +5 < input2.txt 
// int next_tabstop(int col) {
//     for (int i=0; i<MAXSTOPS; ++i) {
//         if (tabstops[i] > col)
//             return tabstops[i];
//     }
//     return col+1;
// }

// int main(int argc, char* argv[]) {
//     // Load command line arguments 
//     int start_col = -1, col_step = -1;
//     for(int i=1; i<argc; i++) {
//         // -m means starting at column m
//         if (argv[i][0] == '-') {
//             get_arg(argv[i]);
//             start_col = atoi(op);
//         }
//         // +n means next column is (m+n),(m+2n),...
//         else if (argv[i][0] == '+') {
//             get_arg(argv[i]);
//             col_step = atoi(op);
//         } else {
//             printf("ERROR: command format wrong!\n");
//         }
//     }

//     // init the loc table
//     if (start_col == -1 || col_step == -1) {
//         start_col = DEFAULT_TAB;
//         col_step = DEFAULT_TAB;
//     }        
//     for (int i=0; i<MAXSTOPS; i++) {
//         tabstops[i] = start_col + i*col_step;
//     }

//     int c, col=0;
//     while((c=getchar()) != EOF) {
//         // we use '!' instead of '\t'
//         if (c=='!') {
//             int next = next_tabstop(col);
//             while(col<next) {
//                 // putchar(' ');
//                 putchar(' ');
//                 ++col;
//             }
//         } else {
//             putchar(c);
//             if (c=='\n')
//                 col = 0;
//             else
//                 ++col;
//         }
//     }
//     return 0;
// }