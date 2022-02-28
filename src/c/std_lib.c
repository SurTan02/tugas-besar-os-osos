#include "header/std_lib.h"
#include "stdio.h"

// Opsional : Tambahkan implementasi kode C
// Operator matematika umum
// Implementasikan
// Tips : Gunakan C integer division
int div(int a, int b)
{
    return a / b;
}

int mod(int a, int n)
{
    return a % n;
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
    char *c = string;
    
    while (c != '\0')
        n++;
    return n;
}


bool strcmp(char *s1, char *s2)
// Mengembalikan true jika string sama
{
    int flag, n;
    char *c1 = s1;
    char *c2 = s2;
    
    flag = strlen(s1) == strlen(s2);
    n = strlen(s1);
    while (flag && n--)
    {
        flag = (*c1 == *c2);
        *c1++;
        *c2++;
    }
    return flag;
}


void strcpy(char *dst, char *src)
// Melakukan penyalinan null terminated string
{
    char *temp = dst;
    // while(*dst++ = *src++); // or 
    while((*dst++=*src++) != '\0');
    // return *temp;
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

