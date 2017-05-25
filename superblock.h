#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <stdlib.h>
#include <cassert>


class SuperBlock
{
    const int userSpaceSize;
    const int blockSize;
    const int blocksNumber;
    const int maxFileNumber;
    int userSpaceInUse;
    int firstINodeIndex;
public:
    SuperBlock(int s=0, int bs=0, int maxFN=0)
        : userSpaceSize(s),
          blockSize(bs),
          blocksNumber((userSpaceSize+blockSize-1)/blockSize),
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
    }

    void decreaseUserSpaceInUse(int val) {
        userSpaceInUse -= val;
        assert(userSpaceInUse >= 0);
    }

    void setFirstINodeIndex(int val) {
        assert(val <= maxFileNumber);
        firstINodeIndex = val;
    }
};

#endif // SUPERBLOCK_H
