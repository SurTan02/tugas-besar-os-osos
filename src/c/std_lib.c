#include "header/std_lib.h"
#include "stdio.h"

// Opsional : Tambahkan implementasi kode C
// Operator matematika umum
// Implementasikan
// Tips : Gunakan C integer division
int div(int a, int b)
{
    return a/b;
}

int mod(int a, int n)
{
	while (a >= n)
		a -= n;
	return a;
}

// Operasi standar bahasa C
// Implementasikan
void memcpy(byte *dest, byte *src, unsigned int n)
// Mengcopy n bytes yang ditunjuk src ke dest
{
    char *c = dest;
    char *s = src;

    while (n--)
    {
        *c++ = *s++;
    }

}

void clear(byte *ptr, unsigned int n)
// Mengosongkan byte array yang memiliki panjang n
{
    int i;
    for (i = 0; i < n ; i++)
    {
        ptr[i] = '\0';
    }
}