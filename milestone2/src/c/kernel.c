// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"
#include "header/filesystem.h"

int main(){
	// fillMap();
    char buf[128];
	
    makeInterrupt21();
    clearScreen();

	printString("  ______        _______.  ______        _______.	\r\n");
	printString(" /  __  \\      /       | /  __  \\      /       | \r\n");
	printString("|  |  |  |    |   (----`|  |  |  |    |   (----`	\r\n");
	printString("|  |  |  |     \\   \\    |  |  |  |    \\   \\   	\r\n");
	printString("|  `--'  | .----)   |   |  `--'  | .----)   |   	\r\n");
	printString(" \\______/  |_______/     \\______/  |_______/    	\r\n");
                                                
    printString("SELAMAT DATANG DI osOS!\r\n");
    printString("TULISKAN NAMA ANDA : ");
	readString(buf);
	printString("Halo ");
	printString(buf);
	printString("\r\n");

	printString("Ketik 'clear' untuk membersihkan layar : !\r\n");
	while (true) 
	{
    	readString(buf);
    	printString(buf);
		printString("\r\n");

		if (strcmp(buf, "clear")){
			break;
		}
	}

    return 0;
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    switch (AX) {
        case 0x0:
            printString(BX);
            break;
        case 0x1:
            readString(BX);
            break;
        case 0x2:
            readSector(BX, CX);
            break;
        case 0x3:
            writeSector(BX, CX);
            break;
        // case 0x4:
        //     read(BX, CX);
        //     break;
        // case 0x5:
        //     write(BX, CX);
        //     break;
        default:
            printString("Invalid Interrupt");
    }
}


void printString (char* word)
{
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
		
		/* Input user berupa backspace */
		else if (currentChar== '\b')
		{
			if (i > 0)
			{
                // Hapus Karakter sebelumnya
                interrupt (0x10, 0x0e * 256 + currentChar, 0, 0, 0);
                interrupt (0x10, 0x0e * 256 + ' ', 0, 0, 0);	
                interrupt (0x10, 0x0e * 256 + currentChar, 0, 0, 0);	
                
                // Karakter yang dihapus direplace nilainya dengan NULL(0)
                string[--i] = '\0';
			}
		}

		/* Normal Input, simpan karakter*/
		else
		{
			string[i++] = currentChar;
			interrupt (0x10, 0x0e * 256 + currentChar, 0, 0, 0);	
		}
	}

	string[i++] = '\0';

	interrupt(0x10, 0xe00 + '\n', 0, 0, 0);		
	interrupt(0x10, 0xe00 + '\r', 0, 0, 0);		
	
}

void clearScreen(){
	// Set video mode 3
	interrupt (0x10, 0x0*256 + 03,  0 , 0, 0);

	// Set warna putih
	interrupt (0x10, 0x06*256 + 0, 0xF*256, 0, 25*256 + 80);
}


void readSector(byte *buffer, int sector_number) {
    int sector_read_count = 0x01;
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) << 8; // CH
    sector   = mod(sector_number, 18) + 1;  // CL

    head  = mod(div(sector_number, 18), 2) << 8; // DH
    drive = 0x00;                                // DL

    interrupt(
        0x13,                       // Interrupt number
        0x0200 | sector_read_count, // AX
        buffer,                     // BX
        cylinder | sector,          // CX
        head | drive                // DX
    );
}


//Harusnya mirip Read cuma beda variabel AX. karena batasan lainnya sama
void writeSector(byte *buffer, int sector_number){
	int sector_read_count = 0x01;
    int cylinder, sector;
    int head, drive;

    cylinder = div(sector_number, 36) << 8; // CH
    sector   = mod(sector_number, 18) + 1;  // CL

    head  = mod(div(sector_number, 18), 2) << 8; // DH
    drive = 0x00;                                // DL

    interrupt(
        0x13,                       // Interrupt number
        0x0300 | sector_read_count, // AX
        buffer,                     // BX
        cylinder | sector,          // CX
        head | drive                // DX
    );
}



// void write(struct file_metadata *metadata, enum fs_retcode *return_code){}

void read(struct file_metadata *metadata, enum fs_retcode *return_code) {
  // Tambahkan tipe data yang dibutuhkan
	struct node_filesystem   node_fs_buffer;
	struct node_entry        node_buffer;
	struct sector_filesystem sector_fs_buffer;
	struct sector_entry      sector_entry_buffer;
	// Asumsikan semua buffer filesystem diatas telah terinisiasi dengan baik

	// Pembacaan storage ke buffer
	readSector(sector_fs_buffer.sector_list, FS_SECTOR_SECTOR_NUMBER);
	readSector(&(node_fs_buffer.nodes[0]),  FS_NODE_SECTOR_NUMBER);
	readSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);

	// Pengcopyan buffer ke entry
	// memcpy(&node_buffer, &(node_fs_buffer.nodes[i]), sizeof(struct node_entry));
	// memcpy(
	// 	&sector_entry_buffer,
	// 	&(sector_fs_buffer.sector_list[i]),
	// 	sizeof(struct sector_entry)
	// );

  // Masukkan filesystem dari storage ke memori buffer

	// 1. Cari node dengan nama dan lokasi yang sama pada filesystem.
	//    Jika ditemukan node yang cocok, lanjutkan ke langkah ke-2.
	//    Jika tidak ditemukan kecocokan, tuliskan retcode FS_R_NODE_NOT_FOUND
	//    dan keluar.  





  // 2. Cek tipe node yang ditemukan
  //    Jika tipe node adalah file, lakukan proses pembacaan.
  //    Jika tipe node adalah folder, tuliskan retcode FS_R_TYPE_IS_FOLDER
  //    dan keluar.

  // Pembacaan
  // 1. memcpy() entry sector sesuai dengan byte S
  // 2. Lakukan iterasi proses berikut, i = 0..15
  // 3. Baca byte entry sector untuk mendapatkan sector number partisi file
  // 4. Jika byte bernilai 0, selesaikan iterasi
  // 5. Jika byte valid, lakukan readSector() 
  //    dan masukkan kedalam buffer yang disediakan pada metadata
  // 6. Lompat ke iterasi selanjutnya hingga iterasi selesai
  // 7. Tulis retcode FS_SUCCESS pada akhir proses pembacaan.
}








// void shell(){}.


//Belum Paham maksudnya hehe
void fillMap() {
  struct map_filesystem map_fs_buffer;
  int i;

  // Load filesystem map
  readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);

  /* 
    Edit filesystem map disini 
                             */

  // Update filesystem map
  writeSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER); 
}
