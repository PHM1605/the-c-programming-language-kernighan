// print file(s) with pagination
// gcc exercise7_8.c
// ./a.out exercise7_8_input1.txt exercise7_8_input2.txt
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINES_PER_PAGE 6
#define HEADER_LINES 3

void print_header(const char* filename, int page) {
    printf("\f");
    printf("%s - Page %d\n", filename, page);
    printf("=======================================\n\n");
}

void print_file(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Cannot open file: %s\n", filename);
        return;
    }

    char line[1024]; // storing reading line
    int line_count = 0;
    int page_number = 1;

    print_header(filename, page_number);
    while(fgets(line, sizeof(line), fp)) {
        if (line_count >= LINES_PER_PAGE - HEADER_LINES) {
            page_number++;
            print_header(filename, page_number);
            line_count = 0;
        }
        fputs(line, stdout);
        line_count++;
    }

    fclose(fp);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file1 [file2...]\n", argv[0]);
        return 1;
    }
    for (int i=1; i<argc; i++) {
        print_file(argv[i]);
    }
    return 0;
}