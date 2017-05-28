#include "filesystem.h"

Filesystem::Filesystem(const char *fn)
    : vdisk(fn)
{ }

static void createFS(const char *fn,
                           int size = DEFAULT_SIZE,
                           int blockSize = DEFALUT_BLOCK_SIZE,
                           int maxFileNumber = DEFAULT_MAX_FILE_NUMBER)
{
    VDisk::createNewVDisk(fn, size, blockSize, maxFileNumber);
}

static void deleteFS(const char *fn) {
    VDisk::deleteVDisk(fn);
}

void Filesystem::copyFileFromLinux(const char *fn) {
    assert(strlen(fn) < FILENAME_LEN);
    if (isFileOnVDisk(fn)) {
        printf("File already exist...\n");
        return;
    }

    FILE *file = fopen(fn, "rb");
    if (!file) {
        printf("File doesn't exist...\n");
        return;
    }

    int fileSize = getFileSize(file);
    SuperBlock superBlock = vdisk.getSuperblock();
    if (fileSize > superBlock.getFreeUserSpace()) {
        printf("File is too big...\n");
        fclose(file);
        return;
    }

    int blockPos = vdisk.getFirstFreeBlockIndex();
    INode inode(fn, fileSize, blockPos);
    int inodePos = vdisk.getFirstFreeINodeIndex();
    vdisk.setInode(inodePos, inode);
    vdisk.setInodeBitmapValue(inodePos, true);
    printf("Saved Filename at %d: %s\n", inodePos, vdisk.getInode(inodePos).getName());

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
    printf("'%s' copied into vdisk...\n", fn);
}

void Filesystem::copyFileFromVDisk(const char *fn) {
    SuperBlock superBlock = vdisk.getSuperblock();
    INode inode;
    int idx = getInodeIndexOfFile(fn);
    if (idx == superBlock.getINodeNumber()) {
        printf("No file with specified filename...\n");
        return;
    }

    FILE *file = fopen(fn, "wb");
    int sizeLeft = inode.getSize();
    const int BLOCK_SIZE = superBlock.getBlockSize();
    int nextBlock = inode.getFirstBlockIndex();
    do {
        Block block = vdisk.getBlock(nextBlock);
        int toReadFromThisBlock = sizeLeft < BLOCK_SIZE ? sizeLeft : BLOCK_SIZE;
        fwrite(block.getData(), toReadFromThisBlock, 1, file);
        sizeLeft -= toReadFromThisBlock;
        nextBlock = block.getNextBlock();
    } while(sizeLeft > 0);
    assert(nextBlock == superBlock.getBlocksNumber());
    fclose(file);

    printf("'%s' copied from vdisk...\n", fn);
}

void Filesystem::ls() {
    printf("Files in dir: \n");
    printf("%15s|%8s|%10s\n", "filename", "size", "1. sector");

    int inodesNumber = vdisk.getSuperblock().getINodeNumber();
    int i = vdisk.getFirstTakenINodeIndex();
    while (i < inodesNumber) {
        INode inode = vdisk.getInode(i);
        printf("%15s|%8d|%10d\n", inode.getName(), inode.getSize(), inode.getFirstBlockIndex());
        i = vdisk.getNextTakenINodeIndex(i);
    }
}

void Filesystem::deleteFile(const char *fn) {
    SuperBlock superBlock = vdisk.getSuperblock();
    int inodeidx = getInodeIndexOfFile(fn);
    if (inodeidx == superBlock.getINodeNumber()) {
        printf("No file with specified filename...\n");
        return;
    }
    INode inode = vdisk.getInode(inodeidx);
    int blockidx = inode.getFirstBlockIndex();

    while (blockidx < superBlock.getBlocksNumber()) {
        vdisk.setBlocksBitmapValue(blockidx, false);
        blockidx = vdisk.getBlock(blockidx).getNextBlock();
    }

    vdisk.setInodeBitmapValue(inodeidx, false);
    printf("'%s' deleted from vdisk...\n", fn);
}

void Filesystem::printStatistics() {
    vdisk.printStatistics();
    vdisk.printSectors();
}

int Filesystem::getFileSize(FILE *f) {
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

int Filesystem::getInodeIndexOfFile(const char *fn) {
    SuperBlock superBlock = vdisk.getSuperblock();
    for (int i = 0, size = superBlock.getINodeNumber(); i < size; ++i) {
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
    int idx = getInodeIndexOfFile(fn);
    int inodesNumber = vdisk.getSuperblock().getINodeNumber();
    return (idx == inodesNumber ? false : true);
}
