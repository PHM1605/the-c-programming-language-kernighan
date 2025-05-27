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
    if((fp->flag & _WRITE) == 0 || ())
}


#define getc(p) (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++: _mfillbuf(p))
#define getchar() getc(mstdin)
#define putc(x,p) (--(p)->cnt>=0 ? (*(p)->ptr++=(x)) : _mflushbuf((x),p))
#define putchar(x) putc((x), mstdout)



int mfclose(MFILE* fp) {
    if (fp == NULL) return EOF;
    int result = 0;
    if (fp->flag & _WRITE)
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
        write(2, "Error opening for write\n", 24); // 2 is fd of stderr
        return 1;
    }

    // push content to that file
    char* msg = "hello world\n";
    for (char* p = msg; *p; p++) {
        putc(*p, out);
    }
    fclose(out);
}