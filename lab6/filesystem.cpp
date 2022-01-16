#include <cstring>
#include <iomanip>

#include "filesystem.h"
#include "constants.h"

void Filesystem::create(std::ofstream &disk)
{
    Superblock superblock;
    superblock.save(disk);

    Inode blankInode;
    for (std::size_t i = 0; i < superblock.n_inodes; i++)
    {
        blankInode.save(disk);
    }

    Datablock blankDatablock;
    for (std::size_t i = 0; i < superblock.n_datablocks; i++)
    {
        blankDatablock.save(disk);
        disk.seekp(DATABLOCK_DATA_SIZE, std::ios_base::cur);
    }
}

Filesystem::Filesystem(std::fstream &file)
    : disk(file), superblock(file) {}

std::vector<Inode> &Filesystem::getInodes()
{
    // lazy-load
    if (are_init_inodes)
        return inodes;

    disk.seekg(superblock.inodes_offset);

    for (std::size_t i = 0; i < superblock.n_inodes; i++)
    {
        inodes.emplace_back(disk);
    }
    are_init_inodes = true;
    return inodes;
}

std::vector<Datablock> &Filesystem::getDatablocks()
{
    // lazy-load
    if (are_init_datablocks)
        return datablocks;

    disk.seekg(superblock.datablocks_offset);

    for (std::size_t i = 0; i < superblock.n_datablocks; i++)
    {
        datablocks.emplace_back(disk);
        disk.seekg(DATABLOCK_DATA_SIZE, std::ios_base::cur);
    }
    are_init_datablocks = true;
    return datablocks;
}

int Filesystem::getFreeInode()
{
    if (getInodes().size() == superblock.n_inodes_taken)
        return -1;

    return superblock.n_inodes_taken;
}

int Filesystem::getFreeDatablock(std::size_t start)
{
    for (std::size_t i = start; i < getDatablocks().size(); i++)
    {
        if (!getDatablocks().at(i).is_taken)
            return i;
    }
    return -1;
}

int Filesystem::getFileInode(const char name[])
{
    for (std::size_t i = 0; i < superblock.n_inodes; i++)
    {
        Inode &inode = getInodes().at(i);
        if (strcmp(inode.name, name) == 0)
            return i;
    }
    return -1;
}

bool Filesystem::is_corrupted()
{
    return superblock.magic_number != MAGIC_NUMBER;
}

bool Filesystem::exportFile(std::ostream &dst, const char name[])
{
    int i_inode = getFileInode(name);
    if (i_inode == -1)
        return false;

    Inode &inode = getInodes().at(i_inode);
    int i_datablock = inode.datablock_first;

    char buffer[DATABLOCK_DATA_SIZE];
    std::size_t n;
    std::size_t total_read = 0;
    while (i_datablock != -1)
    {
        Datablock &datablock = getDatablocks().at(i_datablock);

        disk.seekg(superblock.datablocks_offset + i_datablock * DATABLOCK_SIZE + DATABLOCK_HEADER_SIZE);
        if (total_read + DATABLOCK_DATA_SIZE > inode.file_size)
            n = disk.readsome(buffer, inode.file_size - total_read);
        else
            n = disk.readsome(buffer, DATABLOCK_DATA_SIZE);
        dst.write(buffer, n);
        total_read += n;

        i_datablock = datablock.datablock_next;
    }
    return true;
}

bool Filesystem::importFile(std::istream &src, const char name[])
{
    if (getFileInode(name) != -1)
        return false;
    int i_inode = getFreeInode();
    if (i_inode == -1)
        return false;
    Inode &inode = getInodes().at(i_inode);
    inode.datablock_first = -1;

    int i_datablock = getFreeDatablock();
    if (i_datablock == -1)
        return false;
    Datablock *datablock_prev = &getDatablocks().at(i_datablock);

    char buffer[DATABLOCK_DATA_SIZE];
    std::size_t n;
    std::size_t file_size = 0;

    n = src.readsome(buffer, DATABLOCK_DATA_SIZE);
    file_size += n;
    disk.seekp(superblock.datablocks_offset + i_datablock * DATABLOCK_SIZE + DATABLOCK_HEADER_SIZE);
    disk.write(buffer, n);

    inode.datablock_first = i_datablock;
    datablock_prev->is_taken = true;
    datablock_prev->datablock_next = -1;

    Datablock *datablock;
    while ((n = src.readsome(buffer, DATABLOCK_DATA_SIZE)) != 0)
    {
        // system can read part of the block in the middle of the file
        // read the rest
        std::size_t m;
        while ((m = src.readsome(buffer + n, DATABLOCK_DATA_SIZE - n)) != 0)
        {
            n += m;
        }
        file_size += n;
        if ((i_datablock = getFreeDatablock(i_datablock)) == -1)
        {
            // not enough memory
            // must clean up previous datablocks and the inode

            i_datablock = inode.datablock_first;

            while (i_datablock != -1)
            {
                datablock = &getDatablocks().at(i_datablock);

                datablock->is_taken = false;
                i_datablock = datablock->datablock_next;
                datablock->datablock_next = -1;
            }

            inode.datablock_first = -1;

            return false;
        }
        datablock = &getDatablocks().at(i_datablock);

        disk.seekp(superblock.datablocks_offset + i_datablock * DATABLOCK_SIZE + DATABLOCK_HEADER_SIZE,
                   std::ios_base::beg);
        disk.write(buffer, n);

        datablock_prev->datablock_next = i_datablock;
        datablock->is_taken = true;
        datablock->datablock_next = -1;

        datablock_prev = datablock;
    }

    // succesfully copied whole file
    // now commit changes by saving inode, datablock headers and superblock

    // superblock
    superblock.n_inodes_taken += 1;
    superblock.save(disk);

    // inode
    inode.file_size = file_size;
    strncpy(inode.name, name, MAX_FILENAME_L);
    disk.seekp(superblock.inodes_offset + i_inode * INODE_SIZE);
    inode.save(disk);

    // datablocks
    i_datablock = inode.datablock_first;
    while (i_datablock != -1)
    {
        datablock = &getDatablocks().at(i_datablock);

        disk.seekp(superblock.datablocks_offset + i_datablock * DATABLOCK_SIZE);
        datablock->save(disk);

        i_datablock = datablock->datablock_next;
    }

    return true;
}

bool Filesystem::removeFile(const char name[])
{
    int i_inode_target = getFileInode(name);
    if (i_inode_target == -1)
        return false;

    // remove target inode by swapping with last taken
    // and removing the last taken

    std::size_t i_inode_last = superblock.n_inodes_taken - 1;

    Inode &inode_last = getInodes().at(i_inode_last);
    Inode &inode_target = getInodes().at(i_inode_target);
    int i_datablock = inode_target.datablock_first; // for later

    inode_target = inode_last;

    inode_last.datablock_first = -1;
    inode_last.file_size = 0;
    *inode_last.name = '\0';

    disk.seekp(superblock.inodes_offset + i_inode_target * INODE_SIZE);
    inode_target.save(disk);

    disk.seekp(superblock.inodes_offset + i_inode_last * INODE_SIZE);
    inode_last.save(disk);

    // now remove the datablocks
    while (i_datablock != -1)
    {
        Datablock &datablock = getDatablocks().at(i_datablock);
        disk.seekp(superblock.datablocks_offset + i_datablock * DATABLOCK_SIZE);

        i_datablock = datablock.datablock_next;
        datablock.datablock_next = -1;
        datablock.is_taken = false;

        datablock.save(disk);
    }

    // free up the last inode
    superblock.n_inodes_taken--;
    superblock.save(disk);

    return true;
}

std::size_t Filesystem::takenDatablocks()
{
    std::size_t taken_datablocks = 0;
    for (std::size_t i = 0; i < superblock.n_inodes_taken; i++)
    {
        Inode &inode = getInodes().at(i);
        int i_datablock = inode.datablock_first;
        while (i_datablock != -1)
        {
            taken_datablocks++;
            i_datablock = getDatablocks().at(i_datablock).datablock_next;
        }
    }
    return taken_datablocks;
}

std::size_t Filesystem::wastedSpace()
{
    std::size_t wasted_space = 0;
    for (std::size_t i = 0; i < superblock.n_inodes_taken; i++)
    {
        Inode &inode = getInodes().at(i);
        wasted_space += inode.file_size % DATABLOCK_DATA_SIZE;
    }
    return wasted_space;
}

void Filesystem::listFiles(std::ostream &os)
{
    os << std::left << std::setw(MAX_FILENAME_L) << "NAME"
       << " SIZE\n";

    for (std::size_t i = 0; i < superblock.n_inodes_taken; i++)
    {
        Inode &inode = getInodes().at(i);
        os << std::setw(MAX_FILENAME_L) << inode.name << " " << inode.file_size << "\n";
    }
}

void Filesystem::info(std::ostream &os)
{
    std::size_t taken_datablocks = takenDatablocks();
    os << "Datablocks\t" << taken_datablocks << " / " << superblock.n_datablocks << "\n";
    os << "Inodes\t\t" << superblock.n_inodes_taken << " / " << superblock.n_inodes << "\n";
    os << "Space\t\t" << taken_datablocks * DATABLOCK_DATA_SIZE << " / " << superblock.n_datablocks * DATABLOCK_DATA_SIZE << "\n";
    os << "Wasted space\t" << wastedSpace() << "\n";
    return;
}

void Filesystem::mapBlocks(std::ostream &os)
{
    os << std::left << std::setw(5) << "TYPE"
       << " " << std::left << std::setw(8) << "SIZE"
       << " INFO\n";
    os << std::left << std::setw(5) << "SUPER"
       << " " << std::left << std::setw(8) << SUPERBLOCK_SIZE
       << " -\n";
    for (auto &inode : getInodes())
    {
        os << std::left << std::setw(5) << "INODE"
           << " " << std::left << std::setw(8) << INODE_SIZE
           << " " << inode.name << "\n";
    }
    for (auto &datablock : getDatablocks())
    {
        os << std::left << std::setw(5) << "DATA"
           << " " << std::left << std::setw(8) << DATABLOCK_SIZE
           << " " << (datablock.is_taken ? "TAKEN" : "FREE") << "\n";
    }
}