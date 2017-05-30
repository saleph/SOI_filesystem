#include "vdisk.h"

void VDisk::createNewVDisk(const char *fn,
                           long size,
                           long blockSize,
                           long maxFileNumber)
{
    SuperBlock superBlock(size, blockSize, maxFileNumber);
    long superBlockSize = sizeof(SuperBlock);
    long inodeBitmapSize = superBlock.getINodeNumber();
    long blocksBitmapSize = superBlock.getBlocksNumber();
    long inodesSize = superBlock.getINodeNumber() * (sizeof(INode));
    long rawBlockSize = blockSize + sizeof(long);
    long blocksSize = rawBlockSize * superBlock.getBlocksNumber();
    long wholeFileSize = superBlockSize + inodeBitmapSize + blocksBitmapSize + inodesSize + blocksSize;

    FILE *f = fopen(fn, "wb");
    fwrite((void*)&superBlock, sizeof(SuperBlock), 1, f);
    ftruncate(fileno(f), wholeFileSize); // fills with zeros
    fclose(f);
}

void VDisk::deleteVDisk(const char *fn) {
    unlink(fn);
}

VDisk::VDisk(const char *fn) {
    superBlockOffset = 0;
    strcpy(filename, fn);
    SuperBlock sb = getSuperblock();
    inodeBitmapOffset = sizeof(SuperBlock);
    blocksBitmapOffset = inodeBitmapOffset + sb.getINodeNumber();
    inodesOffset = blocksBitmapOffset + sb.getBlocksNumber();
    blocksOffset = inodesOffset + (sizeof(INode) * sb.getINodeNumber());
    realBlockSize = sb.getBlockSize() + sizeof(long);
}

SuperBlock VDisk::getSuperblock() {
    open();
    SuperBlock sb;
    fread((void*)&sb, sizeof(SuperBlock), 1, file);
    close();
    return sb;
}

void VDisk::open() {
    file = fopen(filename, "r+b");
    if (!file) {
        printf("%d!!!!!!!!!!!!!!!!!!!!! FS DONT OPENED !!!!!!!!!!!!!!!!\n", file);
    }
    rewind(file);
}

void VDisk::close() {
    fclose(file);
}

void VDisk::setSuperBlock(SuperBlock &sb) {
    open();
    fwrite((void*)&sb, sizeof(SuperBlock), 1, file);
    close();
}

bool VDisk::getInodeBitmapValue(long idx) {
    bool val;
    open();
    fseek(file, inodeBitmapOffset + idx, SEEK_SET);
    fread((void*)&val, 1, 1, file);
    close();
    return val;
}

void VDisk::setInodeBitmapValue(long idx, bool val) {
    open();
    fseek(file, inodeBitmapOffset + idx, SEEK_SET);
    fwrite((void*)&val, 1, 1, file);
    close();
}

bool VDisk::getBlocksBitmapValue(long idx) {
    bool val;
    open();
    fseek(file, blocksBitmapOffset + idx, SEEK_SET);
    fread((void*)&val, 1, 1, file);
    close();
    return val;
}

void VDisk::setBlocksBitmapValue(long idx, bool val) {
    open();
    fseek(file, blocksBitmapOffset + idx, SEEK_SET);
    fwrite((void*)&val, 1, 1, file);
    close();
}

INode VDisk::getInode(long idx) {
    INode inode;
    open();
    fseek(file, inodesOffset + (idx*sizeof(INode)), SEEK_SET);
    fread((void*)&inode, sizeof(INode), 1, file);
    close();
    return inode;
}

void VDisk::setInode(long idx, INode &inode) {
    open();
    fseek(file, inodesOffset + (idx*sizeof(INode)), SEEK_SET);
    fwrite((void*)&inode, sizeof(INode), 1, file);
    close();
}

Block VDisk::getBlock(long idx) {
    SuperBlock sb = getSuperblock();
    Block block(sb.getBlockSize());
    long nextBlock;
    open();
    fseek(file, blocksOffset + (idx*realBlockSize), SEEK_SET);
    fread(block.getData(), sb.getBlockSize(), 1, file);
    fread((void*)&nextBlock, sizeof(long), 1, file);
    close();
    block.setNextBlock(nextBlock);
    return block;
}

void VDisk::setBlock(long idx, Block &block) {
    SuperBlock sb = getSuperblock();
    long nextBlock = block.getNextBlock();
    open();
    fseek(file, blocksOffset + (idx*realBlockSize), SEEK_SET);
    fwrite(block.getData(), sb.getBlockSize(), 1, file);
    fwrite((void*)&nextBlock, sizeof(long), 1, file);
    close();
}

long VDisk::getFirstFreeINodeIndex() {
    long size = getSuperblock().getINodeNumber();
    for (long i = 0; i < size; ++i)
    {
        if (getInodeBitmapValue(i) == false) {
            return i;
        }
    }
    return size;
}

long VDisk::getFirstTakenINodeIndex() {
    return getNextTakenINodeIndex(-1);
}

long VDisk::getNextTakenINodeIndex(long idx) {
    long size = getSuperblock().getINodeNumber();
    for (long i = idx + 1; i < size; ++i)
    {
        if (getInodeBitmapValue(i) == true) {
            return i;
        }
    }
    return size;
}

long VDisk::getFirstFreeBlockIndex() {
    return getNextFreeBlockIndex(-1);
}

long VDisk::getNextFreeBlockIndex(long idx) {
    long size = getSuperblock().getBlocksNumber();
    for (long i = idx + 1; i < size; ++i)
    {
        if (getBlocksBitmapValue(i) == false) {
            return i;
        }
    }
    return size;
}

void VDisk::printStatistics() {
    SuperBlock sb = getSuperblock();
    printf("FS statistics:\n");
    printf("%10s|%15s|%10s|%20s\n", "offset", "type", "size", "state [taken/size]");

    printf("%10ld|%15s|%10lu|%9ld/%ld\n", superBlockOffset, "superblock", sizeof(SuperBlock), sb.getUserSpaceInUse(), sb.getUserSpaceSize());
    printf("%10ld|%15s|%10ld|%11s\n", inodeBitmapOffset, "inodes bitmap", sb.getINodeNumber(), "---");
    printf("%10ld|%15s|%10ld|%11s\n", blocksBitmapOffset, "blocks bitmap", sb.getBlocksNumber(), "---");
    printf("%10ld|%15s|%10lu|%9ld/%ld\n", inodesOffset, "inodes", sb.getINodeNumber() * sizeof(INode), getINodesTaken(), sb.getINodeNumber());
    printf("%10ld|%15s|%10ld|%9ld/%ld\n", blocksOffset, "blocks", sb.getBlocksNumber() * realBlockSize, getBlocksTaken(), sb.getBlocksNumber());

}

long VDisk::getINodesTaken() {
    long taken = 0;
    long idx = getFirstTakenINodeIndex();
    long idxMax = getSuperblock().getINodeNumber();
    while (idx < idxMax) {
        ++taken;
        idx = getNextTakenINodeIndex(idx);
    }
    return taken;
}

long VDisk::getBlocksTaken() {
    long taken = 0;
    long idx = getFirstTakenBlockIndex();
    long idxMax = getSuperblock().getBlocksNumber();
    while (idx < idxMax) {
        ++taken;
        idx = getNextTakenBlockIndex(idx);
    }
    return taken;
}

long VDisk::getFirstTakenBlockIndex() {
    return getNextTakenBlockIndex(-1);
}

long VDisk::getNextTakenBlockIndex(long idx) {
    long size = getSuperblock().getBlocksNumber();
    for (long i = idx + 1; i < size; ++i)
    {
        if (getBlocksBitmapValue(i) == true) {
            return i;
        }
    }
    return size;
}

void VDisk::printSectors() {
    SuperBlock sb = getSuperblock();
    printf("================SECTORS:\n");
    printf("===Inodes: \n");

    const long INODES_SECTORS_IN_LINE = sqrt(sb.getINodeNumber());
    for (long i = 0, size = sb.getINodeNumber(); i < size; i++) {
        char c = getInodeBitmapValue(i) ? 'x' : '_';
        printf("%c", c);
        if ((i+1) % INODES_SECTORS_IN_LINE == 0) {
            printf("\n");
        }
    }
    printf("\n");

    printf("===Blocks:\n");
    const long BLOCK_SECTORS_IN_LINE = sqrt(sb.getBlocksNumber());
    for (long i = 0, size = sb.getBlocksNumber(); i < size; i++) {
        char c = getBlocksBitmapValue(i) ? 'x' : '_';
        printf("%c", c);
        if ((i+1) % BLOCK_SECTORS_IN_LINE == 0) {
            printf("\n");
        }
    }
    printf("\n");

}

