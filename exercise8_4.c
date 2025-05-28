#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define PERMS 0666
#define OPEN_MAX 20 // max number of files to be opened

#define _READ 01
#define _WRITE 02
#define _UNBUF 04
#define _EOF 010
#define _ERR 020

typedef struct _iobuf {
    int cnt; // number of chars left in buffer (reading)/ #empty buffer slots (writing)
    char* ptr; // for buffer
    char* base; // buffer name
    int flag;
    int fd; // file descriptor (linking to which file)
} MFILE;

MFILE _iob[OPEN_MAX] = {
    {0, NULL, NULL, _READ, 0},
    {0, NULL, NULL, _WRITE, 1}, 
    {0, NULL, NULL, _WRITE | _UNBUF, 2}
};

#define mstdin (&_iob[0])
#define mstdout (&_iob[1])
#define mstderr (&_iob[2])

int _mflushbuf(int c, MFILE* fp) {
    int bufsize;
    if((fp->flag & _WRITE) == 0 || (fp->flag & (_ERR | _EOF)))
        return EOF;
    bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;
    
    // allocate buffer
    if(fp->base == NULL) {
        if ((fp->base = (char*) malloc(bufsize)) == NULL) {
            fp->flag |= _ERR;
            return EOF;
        }
    } else { // send buffer to file
        int n = fp->ptr - fp->base;
        if (write(fp->fd, fp->base, n)!=n) {
            fp->flag |= _ERR;
            return EOF;
        }
    }
    // here buffer is empty, pushing c to buffer
    fp->cnt = (fp->flag & _UNBUF) ? 0 : bufsize-1;
    fp->ptr = fp->base;
    if(c!=EOF)
        *fp->ptr++ = c;
    return (unsigned char) c;
}

int mfflush(MFILE* fp) {
    if (fp == NULL || (fp->flag & _WRITE) == 0) 
        return 0;
    return _mflushbuf(EOF, fp) == EOF ? EOF : 0;
}

int mfclose(MFILE* fp) {
    if (fp == NULL) return EOF;
    int result = 0;
    if (fp->flag & _WRITE)
        result = mfflush(fp);
    if (close(fp->fd) == -1)
        result = EOF;
    
    free(fp->base);
    fp->cnt = 0;
    fp->ptr = fp->base = NULL;
    fp->flag = 0;
    fp->fd = -1;
    return result;
}

int _mfillbuf(MFILE* fp){
    int bufsize;
    if ((fp->flag & (_READ | _EOF | _ERR)) != _READ)
        return EOF;
    bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;

    if (fp->base == NULL) 
        if((fp->base = (char*)malloc(bufsize)) == NULL)
            return EOF;
    
    fp->ptr = fp->base;
    // offset of file 'fp->fd' is stored in OS kernel
    fp->cnt = read(fp->fd, fp->ptr, bufsize);
    if (--fp->cnt < 0) {
        if (fp->cnt == -1) 
            fp->flag |= _EOF;
        else 
            fp->flag |= _ERR;
        fp->cnt = 0;
        return EOF;
    }
    return (unsigned char)*fp->ptr++;
}

#define mgetc(p) (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++: _mfillbuf(p))
#define mgetchar() mgetc(mstdin)
#define mputc(x,p) (--(p)->cnt>=0 ? (*(p)->ptr++=(x)) : _mflushbuf((x),p))
#define mputchar(x) mputc((x), mstdout)

int mfseek(MFILE* fp, long offset, int origin) {
    if (fp==NULL) return -1;
    if (fp->flag & _WRITE) {
        if (_mflushbuf(EOF, fp) == EOF) return -1;
    } else if (fp->flag & _READ) {
        // because lseek(fd, 0, SEEK_CUR) will move to the file-loc loaded to buffer
        // file-loc is stored in OS Kernel, not seen here
        // e.g. buffer loaded 100 chars, ptr moved to loc=20 (i.e. cnt=80) => we must move back 80
        if (origin == SEEK_CUR)
            offset -= fp->cnt;
        // discard the buffer to force it being loaded again in the future
        fp->cnt = 0;
        fp->ptr = fp->base;
    }
    if (lseek(fp->fd, offset, origin) == -1)
        return -1;
    // after moving we are no longer at EOF => turn that bit off
    fp->flag &= ~_EOF;
    return 0;
}

// if 'open' or 'creat' unsuccessful, return NULL (if pointer) or -1 (if file-descriptor)
MFILE* mfopen(char* name, char* mode) {
    int fd;
    MFILE* fp;

    if(*mode!='r' && *mode!='w' && *mode!='a') return NULL;

    for (fp=_iob; fp<_iob+OPEN_MAX; fp++)
        if((fp->flag & (_READ|_WRITE)) == 0) break;
    if (fp >= _iob+OPEN_MAX) return NULL;

    if (*mode == 'w')
        fd = creat(name, PERMS);
    else if (*mode == 'a') {
        if ((fd = open(name, O_WRONLY, 0)) == -1)
            fd = creat(name, PERMS);
    } else {
        fd = open(name, O_RDONLY, 0);
    }
    if (fd == -1) return NULL;

    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    fp->ptr = NULL;
    fp->flag = (*mode=='r') ? _READ : _WRITE;

    // if append mode, seek to the end of file
    if (*mode == 'a') {
        // file pointer, offset, origin (0:beginning; 1:current; 2:end)
        if (mfseek(fp, 0L, SEEK_END) == -1) {
            mfclose(fp);
            return NULL;
        }
    }
    return fp;
}

int main() {
    MFILE* out = mfopen("exercise8_4_input.txt", "w");
    if (!out) {
        write(2, "Error opening for write\n", 23); // 2 is fd of stderr
        return 1;
    }
    // push content to that file
    char* msg = "hello world\n";
    for (char* p = msg; *p; p++) {
        mputc(*p, out);
    }
    mfclose(out);

    // open that file for reading
    MFILE* in = mfopen("exercise8_4_input.txt", "r");
    if (!in) {
        write(2, "Error opening for read\n", 22);
        return 1;
    }
    // show first 5 chars to screen
    for (int i=0; i<5; i++) {
        int c = mgetc(in);
        write(1, &c, 1);
    }
    // Seek to beginning and show again to screen
    mfseek(in, 0, SEEK_SET);
    write(1, "\nSeeked to beginning:\n", 23);
    int c; 
    while((c=mgetc(in))!=EOF) 
        write(1, &c, 1);
    mfclose(in);
}