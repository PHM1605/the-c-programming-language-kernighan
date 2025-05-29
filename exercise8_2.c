// rewrite the file copying program using ADDITIONAL FIELDS instead of flag-bit operation
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define PERMS 0666
#define OPEN_MAX 20 // max number of files open at once

typedef struct _iobuf {
    int cnt;
    char* ptr;
    char* base;
    int fd;
    char is_read;
    char is_write;
    char is_unbuf;
    char is_eof;
    char is_err;
} MFILE;

MFILE _iob[OPEN_MAX] = {
    {0, NULL, NULL, 0,1,0,0,0,0}, // stdin
    {0, NULL, NULL, 1,0,1,0,0,0}, // stdout
    {0, NULL, NULL, 2,0,1,1,0,0} // stderr (unbuffered)
};



MFILE* mfopen(char* name, char* mode) {
    int fd;
    MFILE* fp;
    if (*mode!='r' && *mode!='w' && *mode!='a')
        return NULL;
    for (fp = _iob; fp<_iob+OPEN_MAX; fp++)
        if(!fp->is_read && !fp->is_write) break; // found free slot
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
    fp->ptr = NULL;
    fp->is_unbuf = 0;
    fp->is_eof = 0;
    fp->is_err = 0;
    fp->is_read = (*mode=='r');
    fp->is_write = (*mode!='r');
    return fp;
}

// (optional) allocate buffer; fill the buffer; get the char from buffer 
int _fillbuf(MFILE* fp) {
    int bufsize;
    // if _EOF or _ERR or _READ not triggered => return 0 to show error
    if (!fp->is_read || fp->is_eof || fp->is_err)
        return EOF;
    // if flag of _UNBUF is on -> return 1
    bufsize = fp->is_unbuf ? 1 : BUFSIZ;
    // if buffer is uninitialized
    if (fp->base == NULL)
        if ((fp->base = (char*) malloc(bufsize)) == NULL)
            return EOF;
    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, bufsize);
    if (--fp->cnt < 0) {
        if (fp->cnt == -1)
            fp->is_eof = 1;
        else 
            fp->is_err = 1;
        fp->cnt = 0;
        return EOF;
    }
    return (unsigned char) *(fp->ptr++);
} 

// (optional) allocate buffer; 
int _flushbuf(int c, MFILE* fp) {
    int bufsize;
    if(!fp->is_write || fp->is_err || fp->is_eof)
        return EOF;
    
    bufsize = fp->is_unbuf ? 1 : BUFSIZ;

    // Allocate buffer if not yet allocated
    if (fp->base == NULL) {
        if ((fp->base = (char*) malloc(bufsize)) == NULL) {
            fp->is_err = 1;
            return EOF;
        }
    } 
    // Write buffer to file
    else {
        int n = fp->ptr - fp->base;
        // file description, destination buffer, #written bytes
        if (write(fp->fd, fp->base, n) != n) {
            fp->is_err = 1;
            return EOF;
        }
    }
    // Buffer here empty in all cases => cnt = bufsize-1 free slots available, after we put 'c' in
    fp->cnt = fp->is_unbuf ? 0 : bufsize-1;
    fp->ptr = fp->base;
    if (c != EOF) {
        *fp->ptr++ = c; // write that character to buffer
    }
    return (unsigned char) c;
}

int mfflush(MFILE* fp) {
    if (fp == NULL || !fp->is_write) return 0;
    // write buffer leftovers to file
    return _flushbuf(EOF, fp) == EOF ? EOF : 0;
}

int mfclose(MFILE* fp) {
    if (fp==NULL) return EOF;
    int result = 0;
    if (fp->is_write) 
        result = mfflush(fp);
    
    // free the connection between file-descriptor & the file
    if (close(fp->fd) == -1) result = EOF;
    free(fp->base);
    fp->cnt = 0;
    fp->ptr = NULL;
    fp->base = NULL;
    fp->is_read = 0;
    fp->is_write = 0;
    fp->is_unbuf = 0;
    fp->is_eof = 0;
    fp->is_err = 0;
    return result;
}

// get a char from buffer OR fill buffer with chars from file then take one out
#define getc(p) (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define getchar() getc(stdin)
// insert a char on the buffer OR if the buffer is full; begin writing to file
#define putc(x,p) (--(p)->cnt >=0 ?  *(p)->ptr++ = (x) : _flushbuf((x), p))
#define putchar(x,p) putc((x), stdout)

int main() {
    MFILE* in = mfopen("exercise8_2_input.txt", "r");
    MFILE* out = mfopen("exercise8_2_output.txt", "w");
    
    if (!in || !out) {
        write(2, "Error opening files\n", 21);
        return 1;
    }
    int c; 
    while((c = getc(in)) != EOF)
        putc(c, out);
    
    mfclose(in);
    mfclose(out);
    return 0;
}