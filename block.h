#ifndef BLOCK_H
#define BLOCK_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cassert>

template <int size>
class Block {
    uint8_t data[size];
    int nextBlock;

public:
    Block()
    { }

    Block(const uint8_t d[size], int next)
        : nextBlock(next)
    {
        memcpy(data, d, sizeof(data));
    }

    const uint8_t &getData() {
        return data;
    }

    void setData(const uint8_t d[size]) {
        memcpy(data, d, sizeof(data));
    }

    int getNextBlock() {
        return nextBlock;
    }

    void setNextBlock(int val) {
        assert(val >= 0);
        nextBlock = val;
    }
};

#endif // BLOCK_H
