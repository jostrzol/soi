#pragma once

#include <fstream>
#include <cstddef>

#include "constants.h"

struct Datablock
{
    int datablock_next = -1;
    bool is_taken = false;

    Datablock();
    Datablock(std::fstream &file);

    void save(std::ostream &file);
    void free();
};
