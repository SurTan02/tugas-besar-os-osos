#include "filesystem.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

// Write 
void write(struct file_metadata *metadata, enum fs_retcode *return_code);

// Read 
void read(struct file_metadata *metadata, enum fs_retcode *return_code);



