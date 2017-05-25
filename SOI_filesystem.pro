TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    vdisk.cpp \
    filesystem.cpp

HEADERS += \
    vdisk.h \
    bitmap.h \
    superblock.h \
    block.h \
    inode.h \
    filesystem.h
