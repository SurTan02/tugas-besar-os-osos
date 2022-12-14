#ifndef _TEXT_IO_H
#define _TEXT_IO_H

#include "std_datatype.h"
#include "std_lib.h"
#include "filesystem.h"

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

int puts(char *string);

int gets(char *string);

void printToInt(int x);

void getArgument(char* input_buf, int *argc, char* argv);

void parser(char* input_buf, char* output1, char* output2);

void printFileContent(char *string);

void printRec(char* path_str, byte current_dir);
void printCWD(char* path_str, byte current_dir);
#endif