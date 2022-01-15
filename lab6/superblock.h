#pragma once

#include <fstream>
#include <cstddef>

#include "constants.h"
#include "datablock.h"
#include "inode.h"

struct Superblock
{
    const unsigned int magic_number = MAGIC_NUMBER;
    const std::size_t fs_size = FS_SIZE;

    const std::size_t inodes_offset = SUPERBLOCK_SIZE;
    const std::size_t n_inodes = N_INODES;
    std::size_t n_inodes_taken = 0;
    const std::size_t inode_size = INODE_SIZE;

    const std::size_t datablocks_offset = SUPERBLOCK_SIZE + N_INODES * INODE_SIZE;
    const std::size_t n_datablocks = N_DATABLOCKS;
    const std::size_t datablock_size = DATABLOCK_SIZE;

    Superblock();
    Superblock(std::fstream &file);

    void save(std::ostream &file);
};
