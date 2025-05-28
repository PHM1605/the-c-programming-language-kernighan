// Program: prints the sizes of all files named in its command line argument list
// if an argument is a directory, print recursively that dir
// if no argument, print the current directory
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "dirent.h"
#include <sys/types.h>
#include <sys/stat.h>
#define MAX_PATH 1024

DIR* opendir(char* dirname) {
    int fd; // file descriptor
    struct stat stbuf;
    DIR* dp;
    // Do DIR allocation (check error in the meantime) sequentially
    if ((fd = open(dirname, O_RDONLY, 0)) == -1
        || fstat(fd, &stbuf) == -1 // same as stat() but using "file descriptor" instead of "name"
        || (stbuf.st_mode & S_IFMT) != S_IFDIR 
        || (dp = (DIR*)malloc(sizeof(DIR))) == NULL 
    )
        return NULL;
    dp->fd = fd;
    return dp;
}

Dirent* readdir(DIR* dp) {
    struct direct dirbuf; // similar to Dirent, defined in <sys/dir.h>
    static Dirent d;
    // read info of dir into 'dirbuf', use it to return 'Dirent pointer'
    while(read(dp->fd, (char*)&dirbuf, sizeof(dirbuf)) == sizeof(dirbuf)) {
        if (dirbuf.d_ino == 0) continue; // slot not in used (file is removed)
        d.ino = dirbuf.d_ino;
        strncpy(d.name, dirbuf.d_name, DIRSIZ);
        d.name[DIRSIZ] = '\0';
        return &d;
    }
    // error in reading dir
    return NULL;
}

void closedir(DIR* dp) {
    if (dp) {
        close(dp->fd);
        free(dp);
    }
}

void dirwalk(char* dir, void (*fcn)(char *)) {
    char name[MAX_PATH]; // fullpath folder/file name
    Dirent* dp;
    DIR* dfd;
    // open the directory
    if ((dfd = opendir(dir)) == NULL) {
        fprintf(stderr, "dirwalk: can't open %s\n", dir);
        return;
    }
    // loop through its content
    while ((dp = readdir(dfd)) != NULL) {
        if (strcmp(dp->name, ".") == 0 || strcmp(dp->name, ".."))
            continue;
        if (strlen(dir)+strlen(dp->name)+2 > sizeof(name))
            fprintf(stderr, "dirwalk: name %s %s too long\n", dir, dp->name);
        else {
            sprintf(name, "%s/%s", dir, dp->name);
            (*fcn)(name);
        }
    }
    closedir(dfd);
}

// fsize: print "size & name" of file "name"
void fsize(char* name) {
    struct stat stbuf;
    // stat: take a "name" and return its stats
    if (stat(name, &stbuf) == -1) {
        fprintf(stderr, "ferr: can't access %s\n", name);
        return;
    }
    // if directory then walk recursively
    if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
        dirwalk(name, fsize);
    printf("%8ld %s\n", stbuf.st_size, name);
}

int main(int argc, char** argv) {
    if (argc == 1) {
        fsize(".");
    } else {
        while(--argc > 0)
            fsize(*++argv);
    }
    return 0;
}