// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"
int main(){
    char buf[128];
    clearScreen();
    makeInterrupt21();

    printString("Halo dunia!\r\n");
    printString("Halo duniassss!\r\n");
    readString(buf);
    printString(buf);
    printString("----------\r\n");
     readString(buf);
    printString(buf);
    printString("----------\r\n");
     readString(buf);
    printString(buf);

    printString("----------\r\n");
     readString(buf);
    printString(buf);



	while (true);
    return 0;
}

void handleInterrupt21 (int AX, int BX, int CX, int DX){
	switch (AX) {
    case 0x0:
      printString(BX);
      break;
    case 0x1:
      readString(BX);
      break;
    default:
      printString("Invalid interrupt");
  }

}

void printString (char* word)
{
	// interrupt(0x10, 0x0e * 256 + '\n', 0, 0, 0);
	while (*word != '\0')
	{
		interrupt (0x10, 0x0e * 256 + *word, 0, 0, 0);
		word++;
	}
}


void readString(char *string)
{
	int i = 0;
	char currentChar = 0;
	while (true)
	{
		currentChar = interrupt (0x16, 0, 0, 0, 0);
		
		/* Input user berupa enter*/
		if (currentChar == 0xd)
        {
            break;
        }
		
		/* Input is backspace */
		else if (currentChar== '\b')
		{
			if (i > 0)
			{
                // Hapus Karakter sebelumnya
                interrupt (0x10, 0x0e * 256 + currentChar, 0, 0, 0);
                interrupt (0x10, 0x0e * 256 + ' ', 0, 0, 0);	
                // interrupt (0x10, 0x0e * 256 + currentChar, 0, 0, 0);	
                
                // Karakter yang dihapus direplace nilainya dengan NULL(0)
                string[i--] = 0;
                // string[i--] = 0;
			}
		}

		/* Normal Input, simpan karakter*/
		else
		{
			string[i++] = currentChar;
			interrupt (0x10, 0x0e * 256 + currentChar, 0, 0, 0);	
		}
	}

	interrupt(0x10, 0xe00 + '\n', 0, 0, 0);		
	interrupt(0x10, 0xe00 + '\r', 0, 0, 0);		
	
}
void clearScreen(){
   int i = 0;
//    char *vidmem = (char *) 0xb8000;
   while (i < 80*25*2){
      putInMemory(0xB000, 0x8000 +  i*2, '\0');
      putInMemory(0xB000, 0x8000 + i*2 +1, 0xF);
    // vidmem[i]=' ';
    // i++;
    // vidmem[i]=0XF;
    i++;
    
   }
}


