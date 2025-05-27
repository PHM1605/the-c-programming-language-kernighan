#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#define PERMS 0666
#define OPEN_MAX 20 // max number of files open at once
#define _READ 01
#define _WRITE 02
#define _UNBUF 04
#define _EOF 010
#define _ERR 020

typedef struct _iobuf {
    int cnt;
    char* ptr;
    char* base;
    int flag;
    int fd;
} MFILE;

MFILE _iob[OPEN_MAX] = {
    {0, (char*)0, (char*)0, _READ, 0},
    {0, (char*)0, (char*)0, _WRITE, 1},
    {0, (char*)0, (char*)0, _WRITE|_UNBUF, 2}
};



MFILE* mfopen(char* name, char* mode) {
    int fd;
    MFILE* fp;
    if (*mode!='r' && *mode!='w' && *mode!='a')
        return NULL;
    for (fp = _iob; fp<_iob+OPEN_MAX; fp++)
        if((fp->flag & (_READ|_WRITE)) == 0) break; // found free slot
    if (fp >= _iob + OPEN_MAX)
        return NULL; // too many files opened
    
    if (*mode == 'w') 
        fd = creat(name, PERMS);
    else if (*mode == 'a') {
        // if append to nonexisting file => create if
        if ((fd = open(name, O_WRONLY, 0)) == -1)
            fd = creat(name, PERMS);
        lseek(fd, 0L, 2); // move to EOF
    } else 
        fd = open(name, O_RDONLY, 0);
    
    if (fd == -1) return NULL; // couldn't access file
    fp->fd = fd;
    fp->cnt = 0; // the next call to getch() => see cnt=0 then trigger _fillbuf
    fp->base = NULL;
    fp->flag = (*mode=='r') ? _READ : _WRITE;
    return fp;
}

int _fillbuf(MFILE* fp) {
    int bufsize;
    // if _EOF or _ERR or _READ not triggered => return 0 to show error
    if ((fp->flag&(_READ|_EOF|_ERR)) != _READ)
        return EOF;
    // if flag of _UNBUF is on -> return 1
    bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
    // if buffer is uninitialized
    if (fp->base == NULL)
        if ((fp->base = (char*) malloc(bufsize)) == NULL)
            return EOF;
    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, bufsize);
    if (--fp->cnt < 0) {
        if (fp->cnt == -1)
            fp->flag |= _EOF;
        else 
            fp->flag |= _ERR;
        fp->cnt = 0;
        return EOF;
    }
    return (unsigned char) *(fp->ptr++);
} 

int _flushbuf(int c, MFILE* fp) {
    int bufsize;
    if((fp->flag & _WRITE) == 0 || (fp->flag & (_ERR | _EOF)))
        return EOF;
    
    bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;

    // Allocate buffer if not yet allocated
    if (fp->base == NULL) {
        if ((fp->base = (char*) malloc(bufsize)) == NULL) {
            fp->flag |= _ERR;
            return EOF;
        }
    } 
    // Write existing buffer to file
    else {
        
    }
}

#define getc(p) (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define getchar() getc(stdin)
#define putc(x,p) (--(p)->cnt >=0 ?  *(p)->ptr++ = (x) : _flushbuf((x), p))
#define putchar(x,p) putc((x), stdout)

int main() {
    MFILE* in = mfopen("input.txt", "r");
    MFILE* out = mfopen("output.txt", "w");
    return 0;
    if (!in || !out) {
        write(2, "Error opening files\n", 21);
        return 1;
    }
    int c; 
    while((c = getc(in)) != EOF)
        putc(c, out);
    
    fclose(in);
    fclose(out);
    return 0;
}