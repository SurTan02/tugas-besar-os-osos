#ifndef KERNEL_H
#define KERNEL_H

#include "std_datatype.h"
#include "std_lib.h"
#include "filesystem.h"
#include "string.h"
#include "message.h"
#include "textio.h"
// #include "util.h"

// Fungsi bawaan
extern void putInMemory(int segment, int address, char character);
extern int interrupt(int int_number, int AX, int BX, int CX, int DX);
extern void launchProgram(int segment);
void executeProgram(struct file_metadata *metadata, int segment, bool *condition);
void makeInterrupt21();
void handleInterrupt21(int AX, int BX, int CX, int DX);
void fillKernelMap();


// TODO : Implementasikan
void printString(char *string);
void printFileContent(char *string);
void readString(char *string);
void printInt(int x);


void clearScreen();

void writeSector(byte *buffer, int sector_number);
void readSector(byte *buffer, int sector_number);

void write(struct file_metadata *metadata, enum fs_retcode *return_code);
void read(struct file_metadata *metadata, enum fs_retcode *return_code);

//Untuk shell
void fillMap();
// void getArgument(char* input_buf, int *argc, char* argv, int maxArg, int size);

#endif