// qsort: sort v[left]...v[right] into increasing order
#include <stdio.h>

void swap(int v[], int i, int j) {
    int temp;
    temp = v[i];
    v[i] = v[j];
    v[j] = temp;
}

void qsort(int v[], int left, int right) {
    int i, last;
    if (left >= right) 
        return;
    swap(v, left, (left+right)/2);
    last = left;
    for (i=left+1; i<=right; i++) {
        if (v[i] < v[left]) {
            swap(v, ++last, i);
        }
    }
    swap(v, left, last);
    qsort(v, left, last-1);
    qsort(v, last+1, right);
}

void print_array(int v[], int n) {
    for (int i=0; i<n; i++) {
        printf("%d%c", v[i], i+1==n ? '\n' : ' ');
    }
}

int main() {
    int v[] = {5,6,9,2,10,1,7};
    int n = sizeof v / sizeof v[0];
    qsort(v, 0, n-1);
    print_array(v, n);
    return 0;
}