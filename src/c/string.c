#include "header/string.h"

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

void substr(char *dst, char *src, int i)
// Melakukan penyalian substring dari i hingga EOF
{
    int j;
    j = 0;
    while (src[i] != '\0') {
        dst[j] = src[i];
        i++; j++;
    }
    dst[j] = '\0';
}