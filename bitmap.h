#ifndef BITMAP_H
#define BITMAP_H

#include <stdint-gcc.h>
#include <stdlib.h>
#include <cstring>

class Bitmap
{
    bool *vector;
    int size;
public:
    Bitmap(int s)
        : vector(new bool[s]), size(s)
    { }
    Bitmap(uint8_t *data, int s)
        : Bitmap(s)
    {
        memcpy(vector, data, size);
    }
    ~Bitmap() {
        delete[] vector;
    }

    bool &operator[](int idx) {
        return vector[idx];
    }

    int getIndexFirstNotZero() {
        for (int i = 0; i < size; i++)
            if (vector[i])
                return i;
        return -1;
    }

    int getSize() {
        return size;
    }

    uint8_t *data() {
        // for copying
        return vector;
    }
};

#endif // BITMAP_H
