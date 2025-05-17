// Define a macro swap(t,x,y) that interchanges two arguments of type t
#include <stdio.h>
#define swap(t, x, y) \
    do {                \
        t tmp = (x);    \
        (x) = (y);      \
        (y) = tmp;      \
    } while(0)

int main() {
    int a = 2, b = 3;
    printf("Originally: a=%d, b=%d\n", a, b);
    swap(int, a, b);
    printf("Swapped: a=%d, b=%d\n", a, b);
}