#define NAME_MAX 14 // longest filename
#define DIRSIZ 1024

typedef struct {
    long ino;
    char name[NAME_MAX+1];
} Dirent;

typedef struct {
    int fd; // file/dir descriptor
    Dirent d;
} DIR;

struct direct {
    ino_t d_ino;
    char d_name[DIRSIZ];
};