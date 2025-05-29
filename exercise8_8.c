// bfree(p,n): manually add an arbitrary block 'p' of size 'n'
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define NALLOC 1024 // we will request from OS at least NALLOC units

typedef long Align;

union header {
    struct {
        union header* ptr; // next block address
        unsigned size; // size of this block
    } s;
    Align x; // this is never used; only to ensure header has size of this field 
};
typedef union header Header;

static Header base; // block with payload=0 at the beginning
static Header* freep = NULL; // start of free list, which is the most recently-modified block

// ap: pointer to multiple-Header-size payload
void mfree(void* ap) {
    if (freep == NULL) {
        base.s.ptr = freep = &base;
        base.s.size = 0;
    }

    Header *p; // returned value; use this to change the value of global 'freep'
    Header *bp = (Header*)ap - 1; // pointer to block header, of the block being freed
    // find the location making bp between p and p-next -> proceeding out
    // initially this block is 'break' immediately 
    for (p=freep; !(bp>p && bp<p->s.ptr); p=p->s.ptr)
        // wrap-around case -> proceeding out if bp out of range
        if (p>=p->s.ptr && (bp>p || bp<p->s.ptr))
            break;
    // join to upper nbr
    if (bp+bp->s.size == p->s.ptr) {
        bp->s.size += p->s.ptr->s.size;
        bp->s.ptr = p->s.ptr->s.ptr;
    } else
        bp->s.ptr = p->s.ptr;
    
    // join to lower nbr 
    if (p+p->s.size == bp) {
        p->s.size += bp->s.size;
        p->s.ptr = bp->s.ptr;
    } else {
        p->s.ptr = bp;
    }
    freep = p;
}

// nu: #units required for BOTH header&payload
static Header* morecore(unsigned nu) {
    Header* up; // what will be returned
    if (nu < NALLOC) nu = NALLOC;
    char* cp = sbrk(nu * sizeof(Header)); 
    // no space at all on OS
    if (cp == (char*)-1) return NULL;
    up = (Header*) cp; // pointer to Header and multiple-Header-size payload
    up->s.size = nu;
    mfree((void*) (up+1));
    return freep;
}

void* mmalloc(unsigned nbytes) {
    Header *p; // to loop over the free-list 
    Header *prevp; // when p moves to next ptr, this will save old value of p

    // request more #units from OS
    Header* morecore(unsigned);
    // e.g. we need 16 bytes, header size of 8 bytes => need 2 payload units and 1 unit for header
    unsigned nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;

    // this block runs only at the very beginning
    if ((prevp = freep) == NULL) {
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }
    // traverse the free list
    for (p=prevp->s.ptr ;; prevp=p, p=p->s.ptr) {
        if (p->s.size >= nunits) {
            if (p->s.size == nunits) 
                prevp->s.ptr = p->s.ptr;
            else {
                p->s.size -= nunits;
                p += p->s.size; // put 'nunits' required at the end of stack
                p->s.size = nunits;
            }
            freep = prevp;
            return (void*)(p+1);
        }
        if (p==freep) 
            if ((p=morecore(nunits)) == NULL)
                return NULL;
    }
}

int bfree(void* p, unsigned n) {
    Header* bp;
    // n must be at least enough to store a Header
    if (n < sizeof(Header)) return 0;

    bp = (Header*) p;
    bp->s.size = n / sizeof(Header);
    mfree((void*)(bp+1));
    return 1;
}

#define STATIC_BYTES 2048
static char static_buf[STATIC_BYTES];

int main() {
    printf("Adding static_buf to empty free list...\n");
    // Creating 2048 bytes
    if (bfree(static_buf, STATIC_BYTES)) {
        printf("Static buffer successfully added to the free list.\n");
    } else {
        printf("Failed to add static buffer. Size too small.\n");
        return 1;
    }

    // Occupying 200 bytes at the end of 2048 bytes => 1848 free bytes
    printf("Trying to allocate 200 bytes using mmalloc...\n");
    void* ptr = mmalloc(200);
    if (ptr) {
        printf("mmalloc(200) succeeded. Address: %p\n", ptr);
        memset(ptr, 0xAB, 200); // fill with pattern to test
    } else {
        printf("mmalloc(200) failed.\n");
        return 1;
    }

    printf("Freeing 200 bytes...\n");
    mfree(ptr); // return 200 bytes back => stack returns to 2048 bytes

    return 0;
}