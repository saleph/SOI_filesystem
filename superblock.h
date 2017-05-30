#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <stdlib.h>
#include <cassert>
#include <cstdio>


class SuperBlock
{
    const long blockSize;
    const long blocksNumber;
    const long userSpaceSize;
    const long maxFileNumber;
    long userSpaceInUse;
    long firstINodeIndex;
public:
    SuperBlock(long s=1, long bs=1, long maxFN=1)
        : blockSize(bs),
          blocksNumber((s+blockSize-1)/blockSize),
          userSpaceSize(blockSize*blocksNumber),
          maxFileNumber(maxFN),
          userSpaceInUse(0),
          firstINodeIndex(maxFileNumber)
    { }

    long getUserSpaceSize() {
        return userSpaceSize;
    }

    long getBlockSize() {
        return blockSize;
    }

    long getBlocksNumber() {
        return blocksNumber;
    }

    long getUserSpaceInUse() {
        return userSpaceInUse;
    }

    long getFreeUserSpace() {
        return userSpaceSize - userSpaceInUse;
    }

    long getFirstINodeIndex() {
        return firstINodeIndex;
    }

    long getINodeNumber() {
        return maxFileNumber;
    }

    void increaseUserSpaceInUse(long val) {
        userSpaceInUse += val;
        assert(userSpaceInUse <= userSpaceSize);
        assert(userSpaceInUse % blockSize == 0);
    }

    void decreaseUserSpaceInUse(long val) {
        userSpaceInUse -= val;
        assert(userSpaceInUse >= 0);
        assert(userSpaceInUse % blockSize == 0);
    }

    void setFirstINodeIndex(long val) {
        assert(val <= maxFileNumber);
        firstINodeIndex = val;
    }

    void printStatistics() {
        printf("Bs: %ld, bn: %ld, maxfn: %ld\n", blockSize, blocksNumber, maxFileNumber);
    }
};

#endif // SUPERBLOCK_H
