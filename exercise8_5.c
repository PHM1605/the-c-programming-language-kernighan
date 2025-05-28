// Program: fsize.c program with additional information
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include "dirent.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h> // for getpwuid()
#include <grp.h> // for getgrgid()
#include <time.h> // for ctime()

#define MAX_PATH 1024

// create a pointer to a DIR object, init 'fd' of it, but 'Dirent' of it is still empty (will be injected in 'readdir')
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
        // return ERROR if 'dirname' is not a directory
        return NULL;
    dp->fd = fd;
    return dp;
}

// dp: pointer of parent folder
// d: 'child' folder/ 'child' file; dirbuf: to get info for 'child'. As 'static', each 'readdir' call overrides the previous one
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
    Dirent* dp; // is of child folder OR child file
    DIR* dfd; // is of parent folder
    // open the directory
    if ((dfd = opendir(dir)) == NULL) {
        fprintf(stderr, "dirwalk: can't open %s\n", dir);
        return;
    }
    // loop through its content
    while ((dp = readdir(dfd)) != NULL) {
        if (strcmp(dp->name, ".") == 0 || strcmp(dp->name, "..")) // skip self and parent 
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

    // File type
    char* type;
    switch (stbuf.st_mode & S_IFMT) {
        case S_IFREG: type="regular"; break;
        case S_IFDIR: type="directory"; break;
        case S_IFLNK: type="symlink"; break;
        case S_IFCHR: type="char device"; break;
        case S_IFBLK: type="block device"; break;
        case S_IFIFO: type="FIFO"; break;
        case S_IFSOCK: type="socket"; break;
        default: type="uknown"; break;
    }

    // Permissions string
    char perms[11];
    perms[0] = S_ISDIR(stbuf.st_mode) ? 'd' : '-';
    perms[1] = (stbuf.st_mode & S_IRUSR) ? 'r' : '-';
    perms[2] = (stbuf.st_mode & S_IWUSR) ? 'w' : '-';
    perms[3] = (stbuf.st_mode & S_IXUSR) ? 'x' : '-'; 
    perms[4] = (stbuf.st_mode & S_IRGRP) ? 'r' : '-';
    perms[5] = (stbuf.st_mode & S_IWGRP) ? 'w' : '-';
    perms[6] = (stbuf.st_mode & S_IXGRP) ? 'x' : '-';
    perms[7] = (stbuf.st_mode & S_IROTH) ? 'r' : '-';
    perms[8] = (stbuf.st_mode & S_IWOTH) ? 'w' : '-';
    perms[9] = (stbuf.st_mode & S_IXOTH) ? 'x' : '-';
    perms[10] = '\0';

    // Get user and group names
    struct passwd* pw = getpwuid(stbuf.st_uid);
    struct group* gr = getgrgid(stbuf.st_gid);
    char* user = pw ? pw->pw_name : "unknown";
    char* group = gr ? gr->gr_name : "unknown";

    printf("%8ld %s %s %3ld %s %s %s", 
        stbuf.st_size, // file size
        perms, // permission
        type, // file type
        (long)stbuf.st_nlink, // #hard links
        user, 
        group, 
        name);
    
    // print modification time
    char* mtime = ctime(&stbuf.st_mtime);
    if (mtime[strlen(mtime)-1] == '\n') {
        mtime[strlen(mtime)-1] = '\0';
        printf(" [%s]\n", mtime);
    }
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