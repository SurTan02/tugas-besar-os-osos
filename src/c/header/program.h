#ifndef _PROGRAM_H
#define _PROGRAM_H

#include "filesystem.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

void executeProgram(struct file_metadata *metadata, int segment);

void exits();

#endif