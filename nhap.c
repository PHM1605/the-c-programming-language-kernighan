int fclose(MFILE* fp) {
    if (fp == NULL)
        return EOF;

    int result = 0;
    if (fp->flag & _WRITE)
        result = fflush(fp);

    if (close(fp->fd) == -1)
        result = EOF;

    if (fp->base)
        free(fp->base);

    fp->cnt = 0;
    fp->ptr = NULL;
    fp->base = NULL;
    fp->flag = 0;
    fp->fd = -1;

    return result;
}

int fflush(MFILE* fp) {
    if (fp == NULL)
        return 0; // ignore NULL
    if ((fp->flag & _WRITE) == 0)
        return 0;
    // write buffer leftovers to file
    return _flushbuf(EOF, fp) == EOF ? EOF : 0;
}

int _flushbuf(int c, MFILE* fp) {
    int bufsize;

    if ((fp->flag & _WRITE) == 0 || (fp->flag & (_ERR | _EOF)))
        return EOF;

    bufsize = (fp->flag & _UNBUF) ? 1 : BUFSIZ;

    if (fp->base == NULL) {
        // Allocate buffer if not yet allocated
        if ((fp->base = (char*) malloc(bufsize)) == NULL) {
            fp->flag |= _ERR;
            return EOF;
        }
    } else {
        // Write existing buffer to file
        int n = fp->ptr - fp->base;
        if (write(fp->fd, fp->base, n) != n) {
            fp->flag |= _ERR;
            return EOF;
        }
    }

    fp->cnt = (fp->flag & _UNBUF) ? 0 : bufsize - 1;
    fp->ptr = fp->base;
    if (c != EOF) {
        *fp->ptr++ = c;
    }

    return (unsigned char) c;
}
