#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define PERMS 0666
#define OPEN_MAX 20

#define _READ  01
#define _WRITE 02
#define _UNBUF 04
#define _EOF   010
#define _ERR   020

typedef struct _iobuf {
    int cnt;
    char* ptr;
    char* base;
    int flag;
    int fd;
} MFILE;

MFILE _iob[OPEN_MAX] = {
    { 0, NULL, NULL, _READ, 0 },
    { 0, NULL, NULL, _WRITE, 1 },
    { 0, NULL, NULL, _WRITE | _UNBUF, 2 }
};

#define stdin  (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2])

#define getc(p) (--(p)->cnt >= 0 ? (unsigned char)*(p)->ptr++ : _fillbuf(p))
#define getchar() getc(stdin)
#define putc(x,p) (--(p)->cnt >= 0 ? (*(p)->ptr++ = (x)) : _flushbuf((x), p))
#define putchar(x) putc((x), stdout)

MFILE* mfopen(char* name, char* mode) {
    int fd;
    MFILE* fp;

    if (*mode != 'r' && *mode != 'w' && *mode != 'a')
        return NULL;

    for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
        if ((fp->flag & (_READ | _WRITE)) == 0)
            break;

    if (fp >= _iob + OPEN_MAX)
        return NULL;

    if (*mode == 'w')
        fd = creat(name, PERMS);
    else if (*mode == 'a') {
        if ((fd = open(name, O_WRONLY, 0)) == -1)
            fd = creat(name, PERMS);
        lseek(fd, 0L, 2);
    } else
        fd = open(name, O_RDONLY, 0);

    if (fd == -1)
        return NULL;

    fp->fd = fd;
    fp->cnt = 0;
    fp->base = NULL;
    fp->ptr = NULL;
    fp->flag = (*mode == 'r') ? _READ : _WRITE;
    return fp;
}

int _fillbuf(MFILE* fp) {
    int bufsize;

    if ((fp->flag & (_READ | _EOF | _ERR)) != _READ)
        return EOF;

    bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;

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

    return (unsigned char)*fp->ptr++;
}

int _flushbuf(int c, MFILE* fp) {
    int bufsize;

    if ((fp->flag & _WRITE) == 0 || (fp->flag & (_ERR | _EOF)))
        return EOF;

    bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;

    if (fp->base == NULL) {
        if ((fp->base = (char*) malloc(bufsize)) == NULL) {
            fp->flag |= _ERR;
            return EOF;
        }
    } else {
        int n = fp->ptr - fp->base;
        if (write(fp->fd, fp->base, n) != n) {
            fp->flag |= _ERR;
            return EOF;
        }
    }

    fp->cnt = (fp->flag & _UNBUF) ? 0 : bufsize - 1;
    fp->ptr = fp->base;

    if (c != EOF)
        *fp->ptr++ = c;

    return (unsigned char)c;
}

int fflush(MFILE* fp) {
    if (fp == NULL || (fp->flag & _WRITE) == 0)
        return 0;
    return _flushbuf(EOF, fp) == EOF ? EOF : 0;
}

int fclose(MFILE* fp) {
    if (fp == NULL)
        return EOF;

    int result = 0;
    if (fp->flag & _WRITE)
        result = fflush(fp);

    if (close(fp->fd) == -1)
        result = EOF;

    free(fp->base);
    fp->cnt = 0;
    fp->ptr = fp->base = NULL;
    fp->flag = 0;
    fp->fd = -1;

    return result;
}

// Bitflag-based fseek
int mfseek(MFILE* fp, long offset, int origin) {
    if (fp == NULL)
        return -1;

    if (fp->flag & _WRITE) {
        if (_flushbuf(EOF, fp) == EOF)
            return -1;
    } else if (fp->flag & _READ) {
        if (origin == SEEK_CUR)
            offset -= fp->cnt;
        fp->cnt = 0;
        fp->ptr = fp->base;
    }

    if (lseek(fp->fd, offset, origin) == -1)
        return -1;

    fp->flag &= ~_EOF;
    return 0;
}
int main() {
    MFILE* out = mfopen("test.txt", "w");
    if (!out) {
        write(2, "Error opening for write\n", 24);
        return 1;
    }

    char* msg = "hello world\n";
    for (char* p = msg; *p; p++)
        putc(*p, out);
    fclose(out);

    MFILE* in = mfopen("test.txt", "r");
    if (!in) {
        write(2, "Error opening for read\n", 23);
        return 1;
    }

    // Read first 5 characters
    for (int i = 0; i < 5; i++) {
        int c = getc(in);
        write(1, &c, 1);
    }

    // Seek to beginning and read again
    mfseek(in, 0, SEEK_SET);
    write(1, "\nSeeked to beginning:\n", 22);

    int c;
    while ((c = getc(in)) != EOF)
        write(1, &c, 1);

    fclose(in);
    return 0;
}
hello
Seeked to beginning:
hello world
