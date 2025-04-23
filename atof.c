// Simplify version of atof in <stdlib.h>
// NOTICE: compiling with "gcc atof.c -lm" to link the math library
#include <stdio.h>
#include <ctype.h>
#include <math.h>

double atof(char s[]) {
    double val, power, exponent;
    int i, j, sign, sign_exp;
    int digit = 0;

    // skip white space at the beginning
    for(i=0; isspace(s[i]); i++) {}
    // handle '+' or '-' sign
    sign = (s[i]=='-') ? -1 : 1;
    if (s[i]=='+' || s[i]=='-')
        i++;
    // handle the part before '.'
    for (val=0.0; isdigit(s[i]); i++) {
        val = 10.0*val + (s[i]-'0');
    }
    if (s[i]=='.')
        i++;
    
    // handle the part after '.'
    for (power=1.0; isdigit(s[i]); i++) {
        val = 10.0 * val + (s[i] - '0');
        power *= 10;
    }
    val = sign * val / power;
    // handle e.g. e-3
    if (s[i]=='e' || s[i]=='E') {
        i++;
        if(s[i] == '-') {
            sign_exp = -1;
            i++;
        } else if (s[i]=='+') {
            sign_exp = 1;
            i++;
        }
        for (j=i; isdigit(s[j]); j++) {
            digit = digit*10+(s[j]-'0');
        }
        val = (sign_exp==1) ? (val*pow(10,digit)) : (val/pow(10,digit));
    }
    return val;
}

int main() {
    double number;
    char number_str[] = "123.45e-6";

    number = atof(number_str);
    printf("%g\n", number);

    return 0;
}