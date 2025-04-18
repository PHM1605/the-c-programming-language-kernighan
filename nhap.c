#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK 1024

typedef struct {
    char type;
    int line;
    int col;
} StackItem;

StackItem stack[MAX_STACK];
int top = -1;

void push(char type, int line, int col) {
    if (top < MAX_STACK - 1) {
        stack[++top].type = type;
        stack[top].line = line;
        stack[top].col = col;
    }
}

StackItem pop() {
    if (top >= 0) {
        return stack[top--];
    } else {
        StackItem empty = {'?', -1, -1};
        return empty;
    }
}

int is_pair(char open, char close) {
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

void check_syntax(FILE *fp) {
    int ch, prev = 0;
    int line = 1, col = 0;
    int in_single_quote = 0, in_double_quote = 0;
    int escape = 0;
    int in_single_comment = 0, in_multi_comment = 0;

    while ((ch = fgetc(fp)) != EOF) {
        col++;

        if (ch == '\n') {
            line++;
            col = 0;
            if (in_single_comment) in_single_comment = 0;
            continue;
        }

        if (escape) {
            escape = 0;
            continue;
        }

        if (ch == '\\') {
            escape = 1;
            continue;
        }

        // Handle comments
        if (!in_single_quote && !in_double_quote) {
            if (!in_single_comment && !in_multi_comment) {
                if (prev == '/' && ch == '/') {
                    in_single_comment = 1;
                } else if (prev == '/' && ch == '*') {
                    in_multi_comment = 1;
                }
            } else if (in_multi_comment && prev == '*' && ch == '/') {
                in_multi_comment = 0;
            }

            if (in_single_comment || in_multi_comment) {
                prev = ch;
                continue;
            }
        }

        // Handle quotes
        if (!in_single_comment && !in_multi_comment) {
            if (ch == '\'' && !in_double_quote) {
                in_single_quote = !in_single_quote;
            } else if (ch == '"' && !in_single_quote) {
                in_double_quote = !in_double_quote;
            }

            if (!in_single_quote && !in_double_quote) {
                if (ch == '(' || ch == '[' || ch == '{') {
                    push(ch, line, col);
                } else if (ch == ')' || ch == ']' || ch == '}') {
                    if (top < 0) {
                        printf("Unmatched closing '%c' at line %d, col %d\n", ch, line, col);
                    } else {
                        StackItem open = pop();
                        if (!is_pair(open.type, ch)) {
                            printf("Mismatched '%c' at line %d, col %d with '%c' at line %d, col %d\n",
                                   open.type, open.line, open.col, ch, line, col);
                        }
                    }
                }
            }
        }

        prev = ch;
    }

    // After EOF
    if (in_single_quote) {
        printf("Unclosed single quote found.\n");
    }
    if (in_double_quote) {
        printf("Unclosed double quote found.\n");
    }
    if (in_multi_comment) {
        printf("Unclosed multi-line comment found.\n");
    }
    while (top >= 0) {
        StackItem open = pop();
        printf("Unmatched opening '%c' at line %d, col %d\n", open.type, open.line, open.col);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source_file.c>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    check_syntax(fp);
    fclose(fp);
    return 0;
}