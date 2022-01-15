#pragma once

#include <fstream>
#include "datablock.h"

#define MAX_FILENAME_L 48

struct Inode
{
    char name[MAX_FILENAME_L] = "\0";
    std::size_t file_size = 0;
    int datablock_first = -1;

    Inode();
    Inode(std::fstream &file);

    void save(std::ostream &file);
};
