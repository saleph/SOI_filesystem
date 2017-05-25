#include "vdisk.h"

void VDisk::createNewVDisk(const char *fn,
                           int size,
                           int blockSize,
                           int maxFileNumber)
{
    SuperBlock superBlock(size, blockSize, maxFileNumber);
    int superBlockSize = sizeof(SuperBlock);
    int inodeBitmapSize = maxFileNumber;
    int blocksBitmapSize = superBlock.getBlocksNumber();
    int rawBlockSize = blockSize + sizeof(int);
    int blocksSize = rawBlockSize * superBlock.getBlocksNumber();
    int wholeFileSize = superBlockSize + inodeBitmapSize + blocksBitmapSize + blocksSize;

    FILE *f = fopen(fn, "wb");
    fwrite((char*)&superBlock, sizeof(superBlock), 1, f);
    ftruncate(fileno(f), wholeFileSize); // fills with zeros
    fclose(f);
}

void VDisk::deleteVDisk(const char *fn) {
    unlink(fn);
}

VDisk::VDisk(const char *fn) {
    strcpy(filename, fn);
    SuperBlock sb = getSuperblock();
    superBlockOffset = 0;
    inodeBitmapOffset = sizeof(SuperBlock);
    blocksBitmapOffset = inodeBitmapOffset + sb.getINodeNumber();
    inodesOffset = blocksBitmapOffset + sb.getBlocksNumber();
    blocksOffset = inodesOffset + sizeof(INode) * sb.getINodeNumber();
    realBlockSize = sb.getBlockSize() + sizeof(int);
}

SuperBlock VDisk::getSuperblock() {
    open();
    SuperBlock sb;
    fseek(file, superBlockOffset, SEEK_SET);
    fread((void*)&sb, sizeof(SuperBlock), 1, file);
    close();
    return sb;
}

void VDisk::setSuperBlock(SuperBlock &sb) {
    open();
    fseek(file, inodeBitmapOffset, SEEK_SET);
    fwrite((void*)&sb, sizeof(SuperBlock), 1, file);
    close();
}

bool VDisk::getInodeBitmapValue(int idx) {
    bool val;
    open();
    fseek(file, inodeBitmapOffset + idx, SEEK_SET);
    fread((void*)&val, 1, 1, file);
    close();
    return val;
}

void VDisk::setInodeBitmapValue(int idx, bool val) {
    open();
    fseek(file, inodeBitmapOffset + idx, SEEK_SET);
    fwrite((void*)&val, 1, 1, file);
    close();
}

bool VDisk::getBlocksBitmapValue(int idx) {
    bool val;
    open();
    fseek(file, blocksBitmapOffset + idx, SEEK_SET);
    fread((void*)&val, 1, 1, file);
    close();
    return val;
}

void VDisk::setBlocksBitmapValue(int idx, bool val) {
    open();
    fseek(file, blocksBitmapOffset + idx, SEEK_SET);
    fwrite((void*)&val, 1, 1, file);
    close();
}

INode VDisk::getInode(int idx) {
    INode inode;
    open();
    fseek(file, inodesOffset + (idx*sizeof(INode)), SEEK_SET);
    fread((void*)&inode, sizeof(INode), 1, file);
    close();
    return inode;
}

void VDisk::setInode(int idx, INode &inode) {
    open();
    fseek(file, inodesOffset + (idx*sizeof(INode)), SEEK_SET);
    fwrite((void*)&inode, sizeof(INode), 1, file);
    close();
}

Block VDisk::getBlock(int idx) {
    SuperBlock sb = getSuperblock();
    Block block(sb.getBlockSize());
    int nextBlock;
    open();
    fseek(file, blocksOffset + (idx*realBlockSize), SEEK_SET);
    fread(block.getData(), sb.getBlockSize(), 1, file);
    fread((void*)&nextBlock, sizeof(int), 1, file);
    close();
    block.setNextBlock(nextBlock);
    return block;
}

void VDisk::setBlock(int idx, Block &block) {
    SuperBlock sb = getSuperblock();
    int nextBlock = block.getNextBlock();
    open();
    fseek(file, blocksOffset + (idx*realBlockSize), SEEK_SET);
    fwrite(block.getData(), sb.getBlockSize(), 1, file);
    fwrite((void*)&nextBlock, sizeof(int), 1, file);
    close();
}

int VDisk::getFirstFreeINodeIndex() {
    for (int i = 0, size = getSuperblock().getINodeNumber();
         i < size; ++i)
    {
        if (getInodeBitmapValue(i) == false) {
            return i;
        }
    }
    return -1;
}

int VDisk::getFirstFreeBlockIndex() {
    return getNextFreeBlockIndex(0);
}

int VDisk::getNextFreeBlockIndex(int idx) {
    for (int i = idx, size = getSuperblock().getBlocksNumber();
         i < size; ++i)
    {
        if (getBlocksBitmapValue(i) == false) {
            return i;
        }
    }
    return -1;
}

void VDisk::open() {
    file = fopen(filename, "r+b");
}

void VDisk::close() {
    fclose(file);
}
