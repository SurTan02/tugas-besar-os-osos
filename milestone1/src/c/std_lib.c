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


unsigned int strlen(char *string)
// Mengembalikan panjang suatu null terminated string
{
    int n = 0;
    while (string[n] != '\0')
        n++;
    return n;
}


bool strcmp(char *s1, char *s2)
// Mengembalikan true jika string sama
{
    int i = 0;
    int flag = true;    
    while (flag)
    {
        if (s1[i] != s2[i]) flag = false;
        if (s1[i] == '\0')  break;
        i++;
    }
    return flag;

}


void strcpy(char *dst, char *src)
// Melakukan penyalinan null terminated string
{
    while((*dst++=*src++) != '\0');
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

