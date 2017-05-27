#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "vdisk.h"
#include <cstdio>
#include <cassert>
#include <unistd.h>
#include "superblock.h"
#include "block.h"
#include "inode.h"
#include "bitmap.h"
#include <cstring>
#include <cmath>

class Filesystem {
    VDisk vdisk;

public:
    Filesystem(const char *fn);

    void copyFileFromLinux(const char *fn);
    void copyFileFromVDisk(const char *fn);
    void ls();
    void deleteFile(const char *fn);
    void printStatistics();

private:
    int getFileSize(FILE *f);
    int getInodeIndexOfFile(const char *fn);
    bool isFileOnVDisk(const char *fn);
};

#endif // FILESYSTEM_H
