#include "filesystem.h"

Filesystem::Filesystem(const char *fn)
    : vdisk(fn)
{ }

void Filesystem::createFS(const char *fn,
                           long size,
                           long blockSize,
                           long maxFileNumber)
{
    VDisk::createNewVDisk(fn, size, blockSize, maxFileNumber);
    printf("\t\t\t\t\t\t\t\t### Filesystem created...\n");
}

void Filesystem::deleteFS(const char *fn) {
    VDisk::deleteVDisk(fn);
    printf("\t\t\t\t\t\t\t\t### Filesystem destroyed...\n");
}

void Filesystem::copyFileFromLinux(const char *fn) {
    assert(strlen(fn) < FILENAME_LEN);
    if (isFileOnVDisk(fn)) {
        printf("\t\t\t\t\t\t\t\t#!# '%s' already exist on vdisk...\n", fn);
        return;
    }

    FILE *file = fopen(fn, "rb");
    if (!file) {
        printf("\t\t\t\t\t\t\t\t#!# '%s' doesn't exist...\n", fn);
        return;
    }

    long fileSize = getFileSize(file);
    SuperBlock superBlock = vdisk.getSuperblock();
    if (fileSize > superBlock.getFreeUserSpace()) {
        printf("\t\t\t\t\t\t\t\t#!# '%s' is too big...\n", fn);
        fclose(file);
        return;
    }

    long blockPos = fileSize == 0 ? superBlock.getBlocksNumber() : vdisk.getFirstFreeBlockIndex();
    INode inode(fn, fileSize, blockPos);
    long inodePos = vdisk.getFirstFreeINodeIndex();
    vdisk.setInode(inodePos, inode);
    vdisk.setInodeBitmapValue(inodePos, true);

    const long BLOCK_SIZE = superBlock.getBlockSize();
    long nextBlockPos;
    long usedSpace = 0;
    Block block(BLOCK_SIZE);
    while(fileSize > 0) {
        fread(block.getData(), 1, BLOCK_SIZE, file);
        fileSize -= BLOCK_SIZE;
        if (fileSize <= 0) {
            // mark end of file
            nextBlockPos = superBlock.getBlocksNumber();
        } else {
            nextBlockPos = vdisk.getNextFreeBlockIndex(blockPos);
        }

        block.setNextBlock(nextBlockPos);
        vdisk.setBlock(blockPos, block);
        vdisk.setBlocksBitmapValue(blockPos, true);
        usedSpace += BLOCK_SIZE;
        blockPos = nextBlockPos;
    }

    superBlock.increaseUserSpaceInUse(usedSpace);
    vdisk.setSuperBlock(superBlock);
    fclose(file);
    printf("\t\t\t\t\t\t\t\t### '%s' copied into vdisk...\n", fn);
}

void Filesystem::copyFileFromVDisk(const char *fn) {
    SuperBlock superBlock = vdisk.getSuperblock();
    long idx = getInodeIndexOfFile(fn);
    if (idx == superBlock.getINodeNumber()) {
        printf("\t\t\t\t\t\t\t\t#!# No file with specified filename...\n");
        return;
    }
    INode inode = vdisk.getInode(idx);


    FILE *file = fopen(fn, "wb");
    long sizeLeft = inode.getSize();
    const long BLOCK_SIZE = superBlock.getBlockSize();
    const long BLOCKS_NO = superBlock.getBlocksNumber();
    long blockIdx = inode.getFirstBlockIndex();
    while(blockIdx < BLOCKS_NO) {
        Block block = vdisk.getBlock(blockIdx);
        long toReadFromThisBlock = sizeLeft < BLOCK_SIZE ? sizeLeft : BLOCK_SIZE;
        fwrite(block.getData(), toReadFromThisBlock, 1, file);
        sizeLeft -= toReadFromThisBlock;
        blockIdx = block.getNextBlock();
    }
    assert(blockIdx == BLOCKS_NO);
    fclose(file);

    printf("\t\t\t\t\t\t\t\t### '%s' copied from vdisk...\n", fn);
}

void Filesystem::ls() {
    printf("---------------------------------------------------------------\n");
    printf("Files in dir: \n");
    printf("%15s|%8s|%10s\n", "filename", "size", "1. sector");

    long inodesNumber = vdisk.getSuperblock().getINodeNumber();
    long i = vdisk.getFirstTakenINodeIndex();
    while (i < inodesNumber) {
        INode inode = vdisk.getInode(i);
        printf("%15s|%8ld|%10ld\n", inode.getName(), inode.getSize(), inode.getFirstBlockIndex());
        i = vdisk.getNextTakenINodeIndex(i);
    }
    printf("===============================================================\n\n");
}

void Filesystem::deleteFile(const char *fn) {
    SuperBlock superBlock = vdisk.getSuperblock();
    long inodeidx = getInodeIndexOfFile(fn);
    if (inodeidx == superBlock.getINodeNumber()) {
        printf("\t\t\t\t\t\t\t\t#!# '%s' doesn't exist on vdisk...\n", fn);
        return;
    }
    INode inode = vdisk.getInode(inodeidx);
    long blockidx = inode.getFirstBlockIndex();

    while (blockidx < superBlock.getBlocksNumber()) {
        vdisk.setBlocksBitmapValue(blockidx, false);
        blockidx = vdisk.getBlock(blockidx).getNextBlock();
    }

    vdisk.setInodeBitmapValue(inodeidx, false);
    printf("\t\t\t\t\t\t\t\t### '%s' deleted from vdisk...\n", fn);
}

void Filesystem::printStatistics() {
    printf("---------------------------------------------------------------\n");
    vdisk.printStatistics();
    printf("===============================================================\n\n");
}

void Filesystem::printSectors() {
    //vdisk.printSectors();
}

long Filesystem::getFileSize(FILE *f) {
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

long Filesystem::getInodeIndexOfFile(const char *fn) {
    SuperBlock superBlock = vdisk.getSuperblock();
    for (long i = 0, size = superBlock.getINodeNumber(); i < size; ++i) {
        if (vdisk.getInodeBitmapValue(i)) {
            INode inode = vdisk.getInode(i);
            if (!strcmp(inode.getName(), fn)) {
                return i;
            }
        }
    }
    return superBlock.getINodeNumber();

}

bool Filesystem::isFileOnVDisk(const char *fn) {
    long idx = getInodeIndexOfFile(fn);
    long inodesNumber = vdisk.getSuperblock().getINodeNumber();
    return (idx == inodesNumber ? false : true);
}
