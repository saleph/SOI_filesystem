#ifndef BITMAP_H
#define BITMAP_H

#include <stdlib.h>
#include <cstring>
#include <cmath>
#include <cstdio>

class Bitmap
{
    bool *vector;
    long size;
public:
    Bitmap(long s)
        : vector(new bool[s]), size(s)
    { }
    Bitmap(uint8_t *data, long s)
        : Bitmap(s)
    {
        memcpy(vector, data, size);
    }
    ~Bitmap() {
        delete[] vector;
    }

    bool &operator[](long idx) {
        return vector[idx];
    }

    long getIndexFirstNotZero() {
        for (long i = 0; i < size; i++)
            if (vector[i])
                return i;
        return -1;
    }

    long getSize() {
        return size;
    }

    uint8_t *data() {
        // for copying
        return (uint8_t*)vector;
    }
};

#endif // BITMAP_H
