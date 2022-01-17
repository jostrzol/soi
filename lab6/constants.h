#pragma once

#include "inode.h"
#include "datablock.h"

#define N_DATABLOCKS (10 * 1024)
#define DATABLOCK_SIZE 1024
#define DATABLOCK_HEADER_SIZE sizeof(Datablock)
#define DATABLOCK_DATA_SIZE (DATABLOCK_SIZE - DATABLOCK_HEADER_SIZE)

#define N_INODES 1024
#define INODE_SIZE sizeof(Inode)

#define SUPERBLOCK_SIZE sizeof(Superblock)

#define MAGIC_NUMBER 0xF234
#define FS_SIZE (SUPERBLOCK_SIZE + N_INODES * INODE_SIZE + N_DATABLOCKS * DATABLOCK_SIZE)