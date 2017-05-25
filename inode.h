#ifndef INODE_H
#define INODE_H

#include <cstring>
#include <cassert>

#define FILENAME_LEN 30

class INode {
    char name[FILENAME_LEN];
    int size;
    int firstBlockIndex;

public:
    INode() { }
    INode(const char *filename, int size, int nextidx)
        : size(size), firstBlockIndex(nextidx)
    {
        strcpy(name, filename);
    }

    const char *getName() {
        return name;
    }

    void setName(const char *val) {
        assert(strlen(val) < FILENAME_LEN);
        strcpy(name, val);
    }

    int getSize() {
        return size;
    }

    void setSize(int val) {
        assert(val >= 0);
        size = val;
    }

    int getFirstBlockIndex() {
        return firstBlockIndex;
    }

    void setFirstBlockIndex(int val) {
        assert(val >= 0);
        firstBlockIndex = val;
    }
};

#endif // INODE_H
