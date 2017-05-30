#include <iostream>
#include "filesystem.h"
#include <cstdio>
#include <cstring>

void doDefault() {
    Filesystem::createFS("lll", 1000000);
    Filesystem("lll").printStatistics();
    Filesystem("lll").copyFileFromLinux("plik0");
    Filesystem("lll").printStatistics();
    Filesystem("lll").ls();
    Filesystem("lll").copyFileFromLinux("plik1");
    Filesystem("lll").printStatistics();
    Filesystem("lll").ls();
    Filesystem("lll").copyFileFromLinux("plik2");
    Filesystem("lll").printStatistics();
    Filesystem("lll").ls();
    Filesystem("lll")
            .copyFileFromLinux("vania.jpg");
    Filesystem("lll")
            .printStatistics();
    Filesystem("lll").ls();
}

int main(int argc, char *argv[])
{
    if(argc<2 || !strcmp(argv[1], "-h")) {
        printf("usage:\n"
               "FS_NAME -c [SIZE] [BLOCK_SIZE] [MAX_FILES_NO]: create FS\n"
               "FS_NAME -v FILENAME: copy from physical disk to FS\n"
               "FS_NAME -m FILENAME: copy from FS to physical disk\n"
               "FS_NAME -l: list files in dir\n"
               "FS_NAME -s: display statistics\n"
               "FS_NAME -d FILENAME: delete file from FS\n"
               "FS_NAME -r: delete FS\n"
               "FS_NAME -i: display taken sectors\n"
               "\n");
        doDefault();
        return -1;
    }

    switch(argv[2][1]) {
        case 'c' :
            switch (argc) {
            case 3:
                Filesystem::createFS(argv[1]);
                return 0;
            case 4:
                Filesystem::createFS(argv[1], atol(argv[3]));
                return 0;
            case 5:
                Filesystem::createFS(argv[1], atol(argv[3]), atol(argv[4]));
                return 0;
            case 6:
                Filesystem::createFS(argv[1], atol(argv[3]), atol(argv[4]), atol(argv[5]));
                return 0;
            }
            return 0;

        case 'v' :
            Filesystem(argv[1]).copyFileFromLinux(argv[3]);
            return 0;

        case 'm' :
            Filesystem(argv[1]).copyFileFromVDisk(argv[3]);
            return 0;

        case 'l' :
            Filesystem(argv[1]).ls();
            return 0;

        case 's' :
            Filesystem(argv[1])
                    .printStatistics();
            return 0;

        case 'r' :
            Filesystem::deleteFS(argv[1]);
            return 0;

        case 'd' :
            Filesystem(argv[1]).deleteFile(argv[3]);
            return 0;

        case 'i' :
            Filesystem(argv[1]).printSectors();
            return 0;

        default :
            printf("ERROR: Command incorrect...\n");
            return -1;
    }


    return 0;
}
