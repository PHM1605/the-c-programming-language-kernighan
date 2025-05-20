// entab: spaces->tab; detab: tab->spaces
// 'entab -m +n' means tab stops every n-columns, starting at column m
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOP 100
char op[MAXOP];
int op_loc = 0;

int is_tabstop(int col) {
    return 0;
}

// start_loc is pointing to '-abc'; writing 'abc' to stack 'op'
void get_arg(char* start_loc) {
    op_loc = 0;
    while(*(++start_loc) != '\0') {
        op[op_loc++] = *start_loc;
    }
    op[op_loc] = '\0';
}

int main(int argc, char* argv[]) {
    int start_col, col_step;
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
    
    // int c; // read char from input file
    // int col = 0; // column indexing
    // int space_count = 0; // space counter
    // while((c=getchar()) != EOF) {
    //     if (c==' ') {
    //         ++space_count;

    //     }
    // }
}