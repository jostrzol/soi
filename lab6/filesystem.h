#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <iostream>

#include "superblock.h"
#include "inode.h"
#include "datablock.h"

class Filesystem
{
private:
    std::fstream &disk;

    Superblock superblock;
    std::vector<Inode> inodes;
    std::vector<Datablock> datablocks;
    bool are_init_inodes = false;
    bool are_init_datablocks = false;

    std::vector<Inode> &getInodes();
    std::vector<Datablock> &getDatablocks();

    int getFreeInode();
    int getFreeDatablock(std::size_t start = 0);
    int getFileInode(const char name[]);

public:
    static void create(std::ofstream &disk);
    Filesystem(std::fstream &disk);

    bool is_corrupted();

    bool exportFile(std::ostream &dst, const char name[]);
    bool importFile(std::istream &src, const char name[]);
    bool removeFile(const char name[]);

    std::size_t takenDatablocks();
    std::size_t wastedSpace();
    void listFiles(std::ostream &os);
    void info(std::ostream &os);
    void mapBlocks(std::ostream &os);
};