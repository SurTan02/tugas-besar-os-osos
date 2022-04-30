#include "header/fileio.h"

void read(struct file_metadata *metadata, enum fs_retcode *return_code)
{
    interrupt(0x21, (metadata->parent_index<<8) + 0x04, metadata, return_code, 0);
}

void write(struct file_metadata *metadata, enum fs_retcode *return_code)
{
    interrupt(0x21, (metadata->parent_index<<8) + 0x04, metadata, return_code, 0);
}

void writeSector(byte *buffer, int sector_number, int parent_index){
    interrupt(0x21, (parent_index<<8) + 0x03, buffer, sector_number, 0);
}

void readSector(byte *buffer, int sector_number, int parent_index){
    interrupt(0x21, (parent_index<<8) + 0x03, buffer, sector_number, 0);
}

void executeProgram(struct file_metadata *metadata, int segment){
    interrupt(0x21, (metadata->parent_index<<8) + 0x06, metadata, segment, 0);
}