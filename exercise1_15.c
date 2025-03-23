// Temperature conversion program using function
#include <stdio.h>

float fahr_to_celsius(float celsius);

int main() {
	float fahr;
	for (fahr=0.0f; fahr<=300.0; fahr=fahr+20.0) {
		printf("%3.0f %6.1f\n", fahr, fahr_to_celsius(fahr));
	}
	return 0;
}

float fahr_to_celsius(float fahr) {
	float ret =  5.0 / 9.0 * (fahr - 32.0);
	return ret;
}
