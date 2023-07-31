#include "datablock.h"
#include "constants.h"

Datablock::Datablock() {}

Datablock::Datablock(std::fstream &file)
{
    file.read(reinterpret_cast<char *>(this), sizeof(Datablock));
}

void Datablock::save(std::ostream &file)
{
    file.write(reinterpret_cast<const char *>(this), DATABLOCK_HEADER_SIZE);
}