#ifndef _STD_LIB_H
#define _STD_LIB_H

#include "std_datatype.h"

// Operator matematika umum
// Implementasikan
// Tips : Gunakan C integer division
int div(int a, int b);

int mod(int a, int n);

// Operasi standar bahasa C
// Implementasikan
void memcpy(byte *dest, byte *src, unsigned int n);
// Mengcopy n bytes yang ditunjuk src ke dest

void clear(byte *ptr, unsigned int n);
// Mengosongkan byte array yang memiliki panjang n

#endif