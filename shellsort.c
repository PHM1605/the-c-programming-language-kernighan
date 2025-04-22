/*
sort v[0], v[1],..., v[n-1] in increasing order
*/
#include <stdio.h>

void shellsort(int v[], int n) {
    int gap, i, j, temp;
    for (gap = n/2; gap>0; gap/=2) {
        for (i=gap; i<n; i++) {
            for (j=i-gap; j>=0 && v[j]>v[j+gap]; j-=gap) {
                // swap v[j] and v[j+gap]
                temp = v[j];
                v[j] = v[j+gap];
                v[j+gap] = temp;
            }
        }
    }
}

int main() {
    int v[] = {2,6,3,4,1};
    int l = sizeof(v)/sizeof(v[0]);
    shellsort(v, l);
    printf("Array after sort:");
    for (int i=0; i<l; i++) {
        printf(" %d", v[i]);
    }
    putchar('\n');
    return 0;
}