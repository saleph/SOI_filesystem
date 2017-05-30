#ifndef VDISK_H
#define VDISK_H

#include <cstdio>
#include <cassert>
#include <unistd.h>
#include "superblock.h"
#include "block.h"
#include "inode.h"
#include <cstdlib>
#include <cstdio>
#include "bitmap.h"
#include <cstring>

#define DEFAULT_SIZE (1 << 20)
#define DEFALUT_BLOCK_SIZE (1 << 8)
#define DEFAULT_MAX_FILE_NUMBER 256

class VDisk
{
    char filename[256];
    FILE *file;
    long superBlockOffset;
    long inodeBitmapOffset;
    long blocksBitmapOffset;
    long inodesOffset;
    long blocksOffset;
    long realBlockSize;
public:
    static void createNewVDisk(const char *fn,
                               long size = DEFAULT_SIZE,
                               long blockSize = DEFALUT_BLOCK_SIZE,
                               long maxFileNumber = DEFAULT_MAX_FILE_NUMBER);
    static void deleteVDisk(const char *fn);

    VDisk(const char *fn);

    SuperBlock getSuperblock();
    void setSuperBlock(SuperBlock &sb);

    bool getInodeBitmapValue(long idx);
    void setInodeBitmapValue(long idx, bool val);

    bool getBlocksBitmapValue(long idx);
    void setBlocksBitmapValue(long idx, bool val);

    INode getInode(long idx);
    void setInode(long idx, INode &inode);

    Block getBlock(long idx);
    void setBlock(long idx, Block &block);

    long getFirstFreeINodeIndex();
    long getFirstTakenINodeIndex();
    long getNextTakenINodeIndex(long idx);

    long getFirstFreeBlockIndex();
    long getNextFreeBlockIndex(long idx);
    long getFirstTakenBlockIndex();
    long getNextTakenBlockIndex(long idx);

    void printStatistics();
    void printSectors();

private:
    void open();
    void close();
    long getINodesTaken();
    long getBlocksTaken();
};

#endif // VDISK_H
