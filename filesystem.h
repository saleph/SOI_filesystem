#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "vdisk.h"
#include <cstdio>
#include <cstring>

class Filesystem {
    VDisk vDisk;
    char filename[50];

public:
    Filesystem(const char *fn) {
        strcpy(filename, fn);
    }

private:
    void
};

#endif // FILESYSTEM_H
