#ifndef INODE_H
#define INODE_H

#include <cstring>
#include <cassert>
#include <cstdint>
#include <climits>
#include <exception>
#include <cstdio>

#define FILENAME_LEN 31

class INode {
    char name[FILENAME_LEN+1];
    long fileSize;
    long firstBlockIndex;

public:
    INode() { }

    INode(const char *filename, long fileSize, long firstidx)
        : fileSize(fileSize), firstBlockIndex(firstidx)
    {
        assert(strlen(filename) <= FILENAME_LEN);
        strcpy(name, filename);
    }

    const char *getName() {
        return name;
    }

    void setName(const char *val) {
        assert(strlen(val) <= FILENAME_LEN);
        strcpy(name, val);
    }

    long getSize() {
        return fileSize;
    }

    void setSize(long val) {
        assert(val >= 0);
        fileSize = val;
    }

    long getFirstBlockIndex() {
        return firstBlockIndex;
    }

    void setFirstBlockIndex(long val) {
        assert(val >= 0);
        firstBlockIndex = val;
    }
};

#endif // INODE_H
