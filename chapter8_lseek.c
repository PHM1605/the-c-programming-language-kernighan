#include <unistd.h>

// get: read 'n_bytes' from position 'pos'
// return number of bytes read (or -1 if error)
int get(int fd, long pos, char* buf, int n) {
    // move to offset 'pos' from beginning
    // return -1 if error, otherwise an integer i.e. new position in the file
    if (lseek(fd, pos, 0) >= 0)
        // read 'n' chars to 'buf'
        return read(fd, buf, n);
    return -1; 
}