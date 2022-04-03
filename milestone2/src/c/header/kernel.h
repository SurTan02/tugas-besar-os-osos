// Kernel header



#include "std_datatype.h"
#include "std_lib.h"
// #include "util.h"

// Fungsi bawaan
extern void putInMemory(int segment, int address, char character);
extern int interrupt(int int_number, int AX, int BX, int CX, int DX);
void makeInterrupt21();
void handleInterrupt21(int AX, int BX, int CX, int DX);
void fillKernelMap();


// TODO : Implementasikan
void printString(char *string);
void readString(char *string);
void clearScreen();

void writeSector(byte *buffer, int sector_number);
void readSector(byte *buffer, int sector_number);

void write(struct file_metadata *metadata, enum fs_retcode *return_code);
void read(struct file_metadata *metadata, enum fs_retcode *return_code);

void printRec(char* path_str, byte current_dir);

void printCWD(char* path_str, byte current_dir);
void shell();

void fillMap();

// void getArgument(char* input, char* arg1, char* arg2, char* arg3, char* arg4);
void list(char current_dir);
void changeDirectory(char dst, char current_dir);
void makeDirectory(byte current_dir, char* arg);
void split(char* input_buf, char* arg1, char* arg2);