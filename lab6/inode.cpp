#include "inode.h"
#include "constants.h"

Inode::Inode() {}

Inode::Inode(std::fstream &file)
{
    file.read(reinterpret_cast<char *>(this), INODE_SIZE);
}

void Inode::save(std::ostream &file)
{
    file.write(reinterpret_cast<const char *>(this), INODE_SIZE);
}
