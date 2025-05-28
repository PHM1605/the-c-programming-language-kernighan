#include <stdio.h>

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

static Header base; // empty list to get started
static Header* freep = NULL; // start of free list

void* malloc(unsigned nbytes) {
    Header *p; // to loop over the free-list 
    Header *prevp; // when p moves to next ptr, this will save old value of p

    // request more #units from OS
    Header* morecore(unsigned nu) {

    }
    unsigned nunits = (nbytes+sizeof(Header)-1)/sizeof(header) + 1;
    // no free list yet
    if ((prevp = freep) == NULL) {
        base.s.ptr = freep = prevp = &base;
        base.s.size = 0;
    }
    for (p=prevp->s.ptr; prevp=p, p=p->s.ptr) {
        if (p->size >= nunits) {
            if (p->s.size == nunits) 
                prevp->s.ptr = p->s.ptr;
            else {
                p->s.size -= nunits;
                p += p->s.size;
                p->s.size = nunits;
            }
            freep = prevp;
            return (void*)(p+1);
        }
    }
}