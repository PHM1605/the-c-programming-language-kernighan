// Write a program to determine the ranges of char, short, int, long, both signed and unsigned. Also floating-point types
#include <stdio.h>
#include <limits.h>
#include <float.h>

void print_integer_ranges_macro() {
    printf("Ranges of integer types using limits.h:\n");
    printf("signed char: %d to %d\n", SCHAR_MIN, SCHAR_MAX);
    printf("unsigned char: 0 to %u\n\n", UCHAR_MAX);

    printf("signed short: %d to %d\n", SHRT_MIN, SHRT_MAX);
    printf("unsigned short: 0 to %u\n\n", USHRT_MAX);

    printf("signed int: %d to %d\n", INT_MIN, INT_MAX);
    printf("unsigned int: 0 to %u\n\n", UINT_MAX);

    printf("signed long: %ld to %ld\n", LONG_MIN, LONG_MAX);
    printf("unsigned long: 0 to %lu\n\n", ULONG_MAX);
}

void print_floating_point_ranges_macro() {
    printf("Ranges of floating-point types using float.h:\n");
    printf("float: min=%.2e, max=%.2e\n", FLT_MIN, FLT_MAX);
    printf("double: min=%.2e, max=%.10e\n", DBL_MIN, DBL_MAX);
    printf("long double: min=%.2Le, max=%.2Le\n", LDBL_MIN, LDBL_MAX);

    printf("\nPrecision info:\n");
    printf("float: digits = %d\n", FLT_DIG);
    printf("double: digit = %d\n", DBL_DIG);
    printf("long double: digits = %d\n", LDBL_DIG);
}

int main() {
    print_integer_ranges_macro();
    print_floating_point_ranges_macro();
    return 0;
}