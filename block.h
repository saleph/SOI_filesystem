#ifndef BLOCK_H
#define BLOCK_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cassert>


class Block {
    long size;
    uint8_t *data;
    long nextBlock;
public:
    Block(long size)
        : size(size), data(new uint8_t[size])
    { }

    ~Block() {
        delete[] data;
    }

    long getNextBlock() {
        return nextBlock;
    }

    void setNextBlock(long val) {
        assert(val >= 0);
        nextBlock = val;
    }

    uint8_t *getData() {
        return data;
    }
};

#endif // BLOCK_H
