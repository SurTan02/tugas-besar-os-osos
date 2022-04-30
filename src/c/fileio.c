#include "header/fileio.h"

void read(struct file_metadata *metadata, enum fs_retcode *return_code)
{
    interrupt(0x21, 0x04, metadata, return_code, 0);
}

void write(struct file_metadata *metadata, enum fs_retcode *return_code)
{
    interrupt(0x21, 0x05, metadata, return_code, 0);
}

void writeSector(byte *buffer, int sector_number){
    interrupt(0x21,  0x03, buffer, sector_number, 0);
}

void readSector(byte *buffer, int sector_number){
    interrupt(0x21,  0x02, buffer, sector_number, 0);
}

void printFileContent(char *string) {
    interrupt(0x21, 0x07, string, 0, 0);
}

// void printReturnCode(char *arg, enum fs_retcode return_code) {
//     interrupt(0x21, 0x08, arg, return_code, 0);
// }