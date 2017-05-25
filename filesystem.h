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

class Filesystem {
    VDisk vdisk;

public:
    Filesystem(const char *fn)
        : vdisk(fn)
    { }

    void copyFileFromLinux(const char *fn) {
        assert(strlen(fn) < FILENAME_LEN);
        if (isFileOnVDisk(fn)) {
            printf("File already on disk...\n");
            return;
        }

        FILE *file = fopen(fn, "rb");
        int fileSize = getFileSize(file);
        SuperBlock superBlock = vdisk.getSuperblock();
        if (fileSize > superBlock.getFreeUserSpace()) {
            printf("File is too big...\n");
            fclose(file);
            return;
        }

        int blockPos = vdisk.getFirstFreeBlockIdx();
        INode inode(fn, fileSize, blockPos);
        int inodePos = vdisk.getFirstFreeINodeIndex();
        vdisk.setInode(inodePos, inode);
        vdisk.setInodeBitmapValue(inodePos, true);

        const int BLOCK_SIZE = superBlock.getBlockSize();
        int nextBlockPos;
        int usedSpace = 0;
        Block block(BLOCK_SIZE);
        while(fileSize > 0) {
            fread(block.getData(), 1, BLOCK_SIZE, file);
            fileSize -= BLOCK_SIZE;
            if (fileSize <= 0) {
                // mark end of file
                block.setNextBlock(superBlock.getBlocksNumber());
            } else {
                nextBlockPos = vdisk.getNextFreeBlockIndex(blockPos);
                block.setNextBlock(nextBlockPos);
            }
            vdisk.setBlock(blockPos, block);
            vdisk.setBlocksBitmapValue(blockPos, true);
            usedSpace += BLOCK_SIZE;
            blockPos = nextBlockPos;
        }

        superBlock.increaseUserSpaceInUse(usedSpace);
        vdisk.setSuperBlock(superBlock);
        fclose(file);
    }

    void copyFileFromVDisk(const char *fn) {

    }

    void ls() {

    }

    void deleteFile(const char *fn) {

    }

    void printStatistics() {

    }

private:
    int getFileSize(FILE *f) {
        fseek(f, 0, SEEK_END);
        int size = ftell(f);
        fseek(f, 0, SEEK_SET);
        return size;
    }

    bool isFileOnVDisk(const char *fn) {

    }


};

#endif // FILESYSTEM_H
