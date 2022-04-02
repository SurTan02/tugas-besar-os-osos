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

void split(char *string, char separator, char splitted[32][15]) {
	int i, j, k;
	i = 0;
	j = 0;
	k = 0;
	clear(splitted[0], 15);

	while (string[i] != '\0') {
		if (string[i] == separator) {
			splitted[j][k] = '\0';
			j += 1;
			k = 0;
			clear(splitted[j], 15);
		} else {
			splitted[j][k] = string[i];
			k += 1;
		}
		i += 1;
	}
	splitted[j][k] = '\0';
}

// void write(struct file_metadata *metadata, enum fs_retcode *return_code){}

void read(struct file_metadata *metadata, enum fs_retcode *return_code) {
	struct node_filesystem   node_fs_buffer;
	struct node_entry        node_buffer;
	struct sector_filesystem sector_fs_buffer;
	struct sector_entry      sector_entry_buffer;
    int    i, parent;
    bool   found;
    
	// Pembacaan storage ke buffer
	readSector(&sector_fs_buffer, FS_SECTOR_SECTOR_NUMBER); 
	readSector(&(node_fs_buffer.nodes[0]),   FS_NODE_SECTOR_NUMBER);        //directory
	readSector(&(node_fs_buffer.nodes[32]) , FS_NODE_SECTOR_NUMBER + 1);    //FILES

    // 1. Cari node dengan nama dan lokasi yang sama pada filesystem.
    //    Jika ditemukan node yang cocok, lanjutkan ke langkah ke-2.
    //    Jika tidak ditemukan kecocokan, tuliskan retcode FS_R_NODE_NOT_FOUND
    //    dan keluar.  

    found = false;
    i = 0;
    while (i < 64 && !found) {
		parent = node_fs_buffer.nodes[i].parent_node_index;
		if((parent == metadata->parent_index) && (strcmp(node_fs_buffer.nodes[i].name, metadata->node_name))){
            found = true;
        } else { 
            i++;
        }
    }

    // 2. Cek tipe node yang ditemukan
    //    Jika tipe node adalah file, lakukan proses pembacaan.
    //    Jika tipe node adalah folder, tuliskan retcode FS_R_TYPE_IS_FOLDER
    //    dan keluar.
    
    if (!found) *return_code = FS_R_NODE_NOT_FOUND;
    else{
        if (node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER) {               //Node saat ini FOLDER
            *return_code = FS_R_TYPE_IS_FOLDER;
        } else {
            // Pembacaan
            // 1. memcpy() entry sector sesuai dengan byte S
            
            // memcpy(&node_buffer, &(node_fs_buffer.nodes[i]), 16);
            memcpy(&node_buffer, &(node_fs_buffer.nodes[i]), sizeof(node_buffer));
            memcpy(&sector_entry_buffer,&sector_fs_buffer.sector_list[node_fs_buffer.nodes[i].sector_entry_index], sizeof(sector_entry_buffer));

            // 2. Lakukan iterasi proses berikut, i = 0..15
            // 3. Baca byte entry sector untuk mendapatkan sector number partisi file
            // 4. Jika byte bernilai 0, selesaikan iterasi
            // 5. Jika byte valid, lakukan readSector() 
            //    dan masukkan kedalam buffer yang disediakan pada metadata
            // 6. Lompat ke iterasi selanjutnya hingga iterasi selesai
            // 7. Tulis retcode FS_SUCCESS pada akhir proses pembacaan.
            i = 0;
            while (sector_entry_buffer.sector_numbers[i] != 0x0 && i < 16) {
                readSector(metadata->buffer, i);
                i++;
            }
            metadata->filesize = i;
            *return_code = FS_SUCCESS;
        }
    }
}








// void shell(){}.


//Belum Paham maksudnya hehe
void fillMap() {
  struct map_filesystem map_fs_buffer;
  int i;

  // Load filesystem map
  readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);

  // Edit filesystem map disini //

   for (i = 0; i < 16; i++){
       map_fs_buffer.is_filled[i] = true;
   }    

   for (i = 256; i < 511; i++){
       map_fs_buffer.is_filled[i] = true;
   }                      

  // Update filesystem map
  writeSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER); 
}
