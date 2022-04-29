#include "header/fileio.h"

void read(struct file_metadata *metadata, enum fs_retcode *return_code)
{
    interrupt(0x21, (metadata->parent_index<<8) + 0x04, metadata, return_code, 0);
}

void write(struct file_metadata *metadata, enum fs_retcode *return_code)
{
    interrupt(0x21, (metadata->parent_index<<8) + 0x04, metadata, return_code, 0);
}