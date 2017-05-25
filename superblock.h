#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <stdlib.h>
#include <cassert>


class SuperBlock
{
    const int blockSize;
    const int blocksNumber;
    const int userSpaceSize;
    const int maxFileNumber;
    int userSpaceInUse;
    int firstINodeIndex;
public:
    SuperBlock(int s=1, int bs=1, int maxFN=1)
        : blockSize(bs),
          blocksNumber((s+blockSize-1)/blockSize),
          userSpaceSize(blockSize*blocksNumber),
          maxFileNumber(maxFN),
          firstINodeIndex(maxFileNumber)
    { }

    int getUserSpaceSize() {
        return userSpaceSize;
    }

    int getBlockSize() {
        return blockSize;
    }

    int getBlocksNumber() {
        return blocksNumber;
    }

    int getUserSpaceInUse() {
        return userSpaceInUse;
    }

    int getFreeUserSpace() {
        return userSpaceSize - userSpaceInUse;
    }

    int getFirstINodeIndex() {
        return firstINodeIndex;
    }

    int getINodeNumber() {
        return maxFileNumber;
    }

    void increaseUserSpaceInUse(int val) {
        userSpaceInUse += val;
        assert(userSpaceInUse <= userSpaceSize);
        assert(userSpaceInUse % blockSize == 0);
    }

    void decreaseUserSpaceInUse(int val) {
        userSpaceInUse -= val;
        assert(userSpaceInUse >= 0);
        assert(userSpaceInUse % blockSize == 0);
    }

    void setFirstINodeIndex(int val) {
        assert(val <= maxFileNumber);
        firstINodeIndex = val;
    }
};

#endif // SUPERBLOCK_H
