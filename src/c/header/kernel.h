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
void executeProgram(struct file_metadata *metadata, int segment);
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
void printRec(char* path_str, byte current_dir);
void printCWD(char* path_str, byte current_dir);
void shell();
void fillMap();
// void getArgument(char* input_buf, int *argc, char* argv, int maxArg, int size);
void list(char current_dir, int argc, char* arg);
void changeDirectory(char dst, char current_dir);
void makeDirectory(byte current_dir, char* arg);
void cat(byte current_dir, char* arg2);
void cp(byte current_dir, char* src, char* dest);
void move(byte current_dir, char* src, char* dst);
void printReturnCode(char *arg, enum fs_retcode return_code);

#endif