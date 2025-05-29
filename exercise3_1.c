// Compare run-time between two 'if's and one 'if' binary search
// v[] is in ascending order of length n
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SIZE 1000000
#define TRIALS 1000000

int binarysearch(int x, int v[], int n) {
    int low=0, high=n-1, mid; 
    while (low<=high) {        
        mid = (low+high)/2;
        if (x < v[mid]) 
            high = mid-1;
        else if (x > v[mid])
            low = mid+1;
        else
            return mid;
    }
    return -1;
}

int binarysearch_one_cmp(int x, int v[], int n) {
    int low = 0, high = n-1, mid;
    while (low <= high) {
        mid = (low+high) / 2;
        int cmp = x - v[mid];
        if (cmp == 0)
            return mid;
        (cmp<0) ? (high=mid-1) : (low=mid+1);
    }
    return -1;
}

int main() {
    int* arr = malloc(sizeof(int) * SIZE);
    // Initialize big array of ints
    for(int i=0; i<SIZE; ++i) 
        arr[i] = i*2;
    int dummy = 0;
    
    clock_t start1 = clock();
    for (int i=0; i<TRIALS; ++i) 
        dummy += binarysearch(i, arr, SIZE);
    clock_t end1 = clock();

    clock_t start2 = clock();
    for (int i=0; i<TRIALS; ++i)
        dummy += binarysearch_one_cmp(i, arr, SIZE);
    clock_t end2 = clock();

    // CLOCKS_PER_SEC: how many ticks per second
    printf("Original: %lf sec\n", (double)(end1 - start1) / CLOCKS_PER_SEC); 
    printf("One cmp: %lf sec\n", (double)(end2 - start2) / CLOCKS_PER_SEC);
    free(arr);

    return 0;
}