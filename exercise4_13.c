#include <stdio.h>
#include <string.h>

void reverse(char s[]) {
    void reverse_recursive(char s[], int left, int right) {
        if (left >= right)
            return;
        // swap 2 elements
        char temp = s[left];
        s[left] = s[right];
        s[right] = temp;

        reverse_recursive(s, left+1, right-1);
    }

    reverse_recursive(s, 0, strlen(s)-1);
}

int main() {
    char s[] = "hello world";
    reverse(s);
    printf("Reversed: %s\n", s);
    return 0;
}