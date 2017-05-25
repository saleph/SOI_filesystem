#include <iostream>
#include "bitmap.h"
#include "block.h"
#include "superblock.h"
#include "vdisk.h"
#include <cstdio>

int main(int argc, char *argv[])
{
    VDisk disk("vdisk");

    printf("%d\n", disk.getSuperblock().getUserSpaceSize());

    /*std::cout << disk.getSuperblock().getUserSpaceSize()
              << std::endl;*/
    return 0;
}
