#ifndef _TEXT_IO_H
#define _TEXT_IO_H

extern int interrupt(int int_number, int AX, int BX, int CX, int DX);

int puts(char *string);

int gets(char *string);
#endif