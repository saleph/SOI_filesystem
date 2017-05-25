#ifndef BLOCK_H
#define BLOCK_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cassert>


class Block {
    int size;
    uint8_t *data;
    int nextBlock;
public:
    Block(int size)
        : size(size), data(new uint8_t[size])
    { }

    ~Block() {
        delete[] data;
    }

    int getNextBlock() {
        return nextBlock;
    }

    void setNextBlock(int val) {
        assert(val >= 0);
        nextBlock = val;
    }

    uint8_t *getData() {
        return data;
    }
};

#endif // BLOCK_H
