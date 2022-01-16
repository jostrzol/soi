#include <fstream>
#include <string>
#include <iostream>

#include "filesystem.h"

int main(int argc, const char *argv[])
{
    if (argc < 3)
        return -1;

    std::string command = argv[1];
    std::string diskname = argv[2];

    if (command == "create")
    {
        if (argc != 3)
            return -1;
        std::ofstream disk{diskname, std::ios::binary | std::ios_base::trunc};
        if (!disk)
        {
            std::cerr << "Could not open file '" << diskname << "'\n";
            return 1;
        }
        Filesystem::create(disk);
        disk.close();
        return 0;
    }

    std::fstream disk{diskname, std::fstream::in | std::fstream::out | std::ios::binary};
    if (!disk)
    {
        std::cerr << "Could not open file '" << diskname << "'\n";
        return 1;
    }
    Filesystem fs(disk);
    if (fs.is_corrupted())
    {
        std::cerr << "Error: corrupted filesystem\n";
    }

    if (command == "ls")
    {
        if (argc != 3)
            return -1;
        fs.listFiles(std::cout);
    }
    else if (command == "cp-to")
    {
        if (argc != 5)
            return -1;
        std::string src = argv[3];
        std::string dst = argv[4];

        std::ifstream fsrc{src, std::ios::binary};
        if (!fsrc)
        {
            std::cerr << "Could not open file '" << src << "'\n";
            return 1;
        }

        if (!fs.importFile(fsrc, dst.c_str()))
        {
            std::cerr << "Copying to filesystem failed. Possible causes:\n";
            std::cerr << "\t* file already exists (check ls)\n";
            std::cerr << "\t* file too large to fit (check info)\n";
            std::cerr << "\t* all inodes taken (check info)\n";
            std::cerr << "\t* others\n";
            return -2;
        }
    }
    else if (command == "cp-from")
    {
        if (argc != 5)
            return -1;
        std::string src = argv[3];
        std::string dst = argv[4];

        std::ofstream fdst{dst, std::ios::binary};
        if (!fdst)
        {
            std::cerr << "Could not open file '" << dst << "'\n";
            return 1;
        }

        if (!fs.exportFile(fdst, src.c_str()))
        {
            std::cerr << "Copying from filesystem failed. Possible causes:\n";
            std::cerr << "\t* file doesn't exist (check ls)\n";
            std::cerr << "\t* others\n";
            return -3;
        }
    }
    else if (command == "rm")
    {
        if (argc != 4)
            return -1;
        std::string file = argv[3];

        if (!fs.removeFile(file.c_str()))
        {
            std::cerr << "Removing from filesystem failed. Possible causes:\n";
            std::cerr << "\t* file doesn't exist (check ls)\n";
            std::cerr << "\t* others\n";
            return -4;
        }
    }
    else if (command == "info")
    {
        if (argc != 3)
            return -1;

        fs.info(std::cout);
    }
    else if (command == "map")
    {
        if (argc != 3)
            return -1;

        fs.mapBlocks(std::cout);
    }
    else
    {
        std::cerr << "Unrecognized command '" << command << "'. Possible commands:\n";
        std::cerr << "\t* create FILEPATH\t\tcreates an empty filesystem\n";
        std::cerr << "\t* cp-to FILESYSTEM SRC DST\tcopies a file from an external filesystem\n";
        std::cerr << "\t* cp-from FILESYSTEM SRC DST\tcopies a file to the external filesystem\n";
        std::cerr << "\t* rm FILESYSTEM FILE\t\tremoves a file\n";
        std::cerr << "\t* ls FILESYSTEM\t\t\tlists all the files\n";
        std::cerr << "\t* info FILESYSTEM\t\tlists information about filesystem\n";
        std::cerr << "\t* map FILESYSTEM\t\tlists all the blocks in the filesystem\n";
        return -5;
    }

    disk.close();
    return 0;
}
