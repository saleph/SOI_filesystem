#ifndef VDISK_H
#define VDISK_H

#include <cstdio>
#include <cassert>
#include <unistd.h>
#include "superblock.h"
#include "block.h"
#include "inode.h"
#include "bitmap.h"

#define DEFAULT_SIZE (1 << 20)
#define DEFALUT_BLOCK_SIZE (1 << 8)
#define DEFAULT_MAX_FILE_NUMBER 256

class VDisk
{
    char filename[256];
    FILE *file;
    int superBlockOffset;
    int inodeBitmapOffset;
    int blocksBitmapOffset;
    int inodesOffset;
    int blocksOffset;
public:
    static void createNewVDisk(const char *fn,
                               int size = DEFAULT_SIZE,
                               int blockSize = DEFALUT_BLOCK_SIZE,
                               int maxFileNumber = DEFAULT_MAX_FILE_NUMBER)
    {
        SuperBlock superBlock(size, blockSize, maxFileNumber);
        int superBlockSize = sizeof(SuperBlock);
        int inodeBitmapSize = maxFileNumber;
        int blocksBitmapSize = superBlock.getBlocksNumber();
        int blocksSize = blockSize * superBlock.getBlocksNumber();
        int wholeFileSize = superBlockSize + inodeBitmapSize + blocksBitmapSize + blocksSize;

        FILE *f = fopen(fn, "wb");
        fwrite((char*)&superBlock, sizeof(superBlock), 1, f);
        ftruncate(fileno(f), wholeFileSize); // fills with zeros
        fclose(f);
    }

    VDisk(const char *fn) {
        strcpy(filename, fn);
        SuperBlock sb = getSuperblock();
        superBlockOffset = 0;
        inodeBitmapOffset = sizeof(SuperBlock);
        blocksBitmapOffset = inodeBitmapOffset + sb.getINodeNumber();
        inodesOffset = blocksBitmapOffset + sb.getBlocksNumber();
        blocksOffset = inodesOffset + sizeof(INode) * sb.getINodeNumber();
    }

    SuperBlock getSuperblock() {
        open();
        SuperBlock sb;
        fseek(file, superBlockOffset, SEEK_SET);
        fread((void*)&sb, sizeof(SuperBlock), 1, file);
        close();
        return sb;
    }

    void setSuperBlock(SuperBlock &sb) {
        open();
        fseek(file, inodeBitmapOffset, SEEK_SET);
        fwrite((void*)&sb, sizeof(SuperBlock), 1, file);
        close();
    }

    bool getInodeBitmapValue(int idx) {
        bool val;
        open();
        fseek(file, inodeBitmapOffset + idx, SEEK_SET);
        fread((void*)&val, 1, 1, file);
        close();
        return val;
    }

    void setInodeBitmapValue(int idx, bool val) {
        open();
        fseek(file, inodeBitmapOffset + idx, SEEK_SET);
        fwrite((void*)&val, 1, 1, file);
        close();
    }

    bool getBlocksBitmapValue(int idx) {
        bool val;
        open();
        fseek(file, blocksBitmapOffset + idx, SEEK_SET);
        fread((void*)&val, 1, 1, file);
        close();
        return val;
    }

    void setBlocksBitmapValue(int idx, bool val) {
        open();
        fseek(file, blocksBitmapOffset + idx, SEEK_SET);
        fwrite((void*)&val, 1, 1, file);
        close();
    }

    INode getInode(int idx) {
        INode inode;
        open();
        fseek(file, inodesOffset + (idx*sizeof(INode)), SEEK_SET);
        fread((void*)&inode, sizeof(INode), 1, file);
        close();
        return inode;
    }

    void setInode(int idx, INode &inode) {
        open();
        fseek(file, inodesOffset + (idx*sizeof(INode)), SEEK_SET);
        fwrite((void*)&inode, sizeof(INode), 1, file);
        close();
    }

    Block getBlock(int idx) {
        Block block;
        open();
        fseek(file, blocksOffset + (idx*sizeof(Block)), SEEK_SET);
        fread((void*)&block, sizeof(Block), 1, file);
        close();
        return block;
    }

    void setBlock(int idx, Block &block) {
        open();
        fseek(file, blocksOffset + (idx*sizeof(Block)), SEEK_SET);
        fwrite((void*)&block, sizeof(Block), 1, file);
        close();
    }

private:
    void open() {
        file = fopen(filename, "r+b");
    }
    void close() {
        fclose(file);
    }


};

#endif // VDISK_H
