#ifndef _FILE_IO_H
#define _FILE_IO_H

#include "filesystem.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

// Write 
void write(struct file_metadata *metadata, enum fs_retcode *return_code);

// Read 
void read(struct file_metadata *metadata, enum fs_retcode *return_code);

void writeSector(byte *buffer, int sector_number, int parent_index);
   
void readSector(byte *buffer, int sector_number, int parent_index);

void executeProgram(struct file_metadata *metadata, int segment);

#endif