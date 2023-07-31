#include <fstream>
#include "superblock.h"

Superblock::Superblock() {}

Superblock::Superblock(std::fstream &file)
{
    file.seekg(0, std::ios_base::beg);
    file.read(reinterpret_cast<char *>(this), SUPERBLOCK_SIZE);
}

void Superblock::save(std::ostream &file)
{
    file.seekp(0, std::ios_base::beg);
    file.write(reinterpret_cast<const char *>(this), SUPERBLOCK_SIZE);
}
