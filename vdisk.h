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
    int realBlockSize;
public:
    static void createNewVDisk(const char *fn,
                               int size = DEFAULT_SIZE,
                               int blockSize = DEFALUT_BLOCK_SIZE,
                               int maxFileNumber = DEFAULT_MAX_FILE_NUMBER);
    static void deleteVDisk(const char *fn);

    VDisk(const char *fn);

    SuperBlock getSuperblock();
    void setSuperBlock(SuperBlock &sb);

    bool getInodeBitmapValue(int idx);
    void setInodeBitmapValue(int idx, bool val);

    bool getBlocksBitmapValue(int idx);
    void setBlocksBitmapValue(int idx, bool val);

    INode getInode(int idx);
    void setInode(int idx, INode &inode);

    Block getBlock(int idx);
    void setBlock(int idx, Block &block);

    int getFirstFreeINodeIndex();
    int getFirstFreeBlockIndex();
    int getNextFreeBlockIndex(int idx);

private:
    void open();
    void close();
};

#endif // VDISK_H
