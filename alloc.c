#define ALLOCSIZE 10000
static char allocbuf[ALLOCSIZE];
static char* allocp = allocbuf; 

// returns pointer to where going to assign n elements
char* alloc(int n) {
    if (allocbuf + ALLOCSIZE - allocp >=n) {
        allocp += n;
        return allocp - n;
    } else {
        return 0; // return 0 if not enough
    }
}

// free storage pointed to by p
void afree(char* p) {
    if (p>=allocbuf && p<allocbuf + ALLOCSIZE) {
        allocp = p;
    }

}