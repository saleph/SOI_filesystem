#include <iostream>
#include "bitmap.h"
#include "block.h"
#include "superblock.h"
#include "filesystem.h"
#include "vdisk.h"
#include <cstdio>

int main(int argc, char *argv[])
{
    //VDisk::createNewVDisk("dd");
    Filesystem fs("dd");
    //fs.copyFileFromLinux("file.txt");
    //fs.printStatistics();
    //fs.copyFileFromVDisk("file.txt");
    fs.ls();
    fs.deleteFile("file.txt");
    fs.ls();

    return 0;
}
