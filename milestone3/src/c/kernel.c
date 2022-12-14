// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"

int main() {
    
	fillMap();
	
    makeInterrupt21();
    clearScreen();    
    interrupt (0x10, 0x06*256 + 0, 0xC*256, 0, 6*256 + 31);
	interrupt (0x10, 0x06*256 + 0, 0xB*256, 0, 5*256 + 80);
	printString("  ______        _______.  ______        _______.	\r\n");
	printString(" /  __  \\      /       | /  __  \\      /       | \r\n");
	printString("|  |  |  |    |   (----`|  |  |  |    |   (----`	\r\n");
	printString("|  |  |  |     \\   \\    |  |  |  |    \\   \\   	\r\n");
	printString("|  `--'  | .----)   |   |  `--'  | .----)   |   	\r\n");
	printString(" \\______/  |_______/     \\______/  |_______/    	\r\n");

    
    printString("Init your shell and press ENTER!"); readString("a");
	while (true) 
	{
        // shell();
        struct message msg;
        struct file_metadata meta;

        msg.current_directory = 0xFF;
        setMessage(&msg);

        meta.node_name    = "shell";
        meta.parent_index = 0;
        executeProgram(&meta, 0x2000, true);
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
        case 0x4:
            read(BX, CX);
            break;
        case 0x5:
            write(BX, CX);
            break;
        case 0x6:
            executeProgram(BX, CX, DX);
            break;
        case 0x7:
            printFileContent(BX);
            break;
        default:
            printString("Invalid Interrupt");
    }
}

void executeProgram(struct file_metadata *metadata, int segment, bool *condition) {
  enum fs_retcode fs_ret;
  byte buf[8192];
   
  metadata->buffer = buf;
  read(metadata, &fs_ret);
  if (fs_ret == FS_SUCCESS) {
    int i = 0;
    for (i = 0; i < 8192; i++) {
      if (i < metadata->filesize)
        putInMemory(segment, i, metadata->buffer[i]);
      else
        putInMemory(segment, i, 0x00);
    }
    *condition = true;
    launchProgram(segment);
  }
  else
    // printString("exec: file not found\r\n");
    *condition = false;
}

void printInt(int x) {
    char string[16];
    char reverse[16];
    int i;
    
    if (x == 0) {
        string[0] = x + '0';
        string[1] = '\0';
    } else {
        i = 0;
        while (x != 0) {
            string[i] = mod(x, 10) + '0';
            x = div(x, 10);
            i++;
        }
        string[i] = '\0';
    }

    for (i = strlen(string) - 1; i > -1; i--) {
        reverse[strlen(string) - 1 - i] = string[i];
    }
    reverse[strlen(string)] = '\0';

    printString(reverse);
}

void printString (char* word) {
	while (*word != '\0')
	{
		interrupt (0x10, 0x0e * 256 + *word, 0, 0, 0);		
		word++;
	}
}

void printFileContent(char *string) {
	while (*string != '\0')
	{
        if (*string == '\n') {
            interrupt (0x10, 0x0e * 256 + '\r', 0, 0, 0);
        }
        		
        interrupt (0x10, 0x0e * 256 + *string, 0, 0, 0);
        string++;
	}
}

void readString(char *string) {
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

void clearScreen() {
	// Set video mode 3
	interrupt (0x10, 0x0*256 + 03, 0, 0, 0);

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
void writeSector(byte *buffer, int sector_number) {
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

void read(struct file_metadata *metadata, enum fs_retcode *return_code) {
	struct node_filesystem   node_fs_buffer;
	struct sector_filesystem sector_fs_buffer;
	struct sector_entry      sector_entry_buffer;
    int    i,j;
    bool   found;
    
    
    
    // clear(buf, 8192);
	// Pembacaan storage ke buffer
	readSector(&sector_fs_buffer, FS_SECTOR_SECTOR_NUMBER); 
	readSector(&(node_fs_buffer.nodes[0]),   FS_NODE_SECTOR_NUMBER);       
	readSector(&(node_fs_buffer.nodes[32]) , FS_NODE_SECTOR_NUMBER + 1);   

    // 1. Cari node dengan nama dan lokasi yang sama pada filesystem.
    found = false;
    i = 0;
    while (i < 64 && !found) {
		if((node_fs_buffer.nodes[i].parent_node_index == metadata->parent_index) && (strcmp(node_fs_buffer.nodes[i].name, metadata->node_name))){
            found = true;
        } else { 
            i++;
        }
    }

    // 2. Cek tipe node yang ditemukan
    //    Jika tidak ditemukan kecocokan, tuliskan retcode FS_R_NODE_NOT_FOUND dan keluar. 
    if (!found) *return_code = FS_R_NODE_NOT_FOUND;
    else{
        // Jika tipe node adalah folder, tuliskan retcode FS_R_TYPE_IS_FOLDER
        if (node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER) {               //Node saat ini FOLDER
            *return_code = FS_R_TYPE_IS_FOLDER;

        //    Jika tipe node adalah file, lakukan proses pembacaan.
        } else {
            // Pembacaan
            // 1. memcpy() entry sector sesuai dengan byte S
            memcpy(&sector_entry_buffer,&sector_fs_buffer.sector_list[node_fs_buffer.nodes[i].sector_entry_index], sizeof(sector_entry_buffer));
            
            // 2. Lakukan iterasi proses berikut, i = 0..15
            // 3. Baca byte entry sector untuk mendapatkan sector number partisi file
            // 4. Jika byte bernilai 0, selesaikan iterasi
            // 5. Jika byte valid, lakukan readSector() 
            //    dan masukkan kedalam buffer yang disediakan pada metadata
            // 6. Lompat ke iterasi selanjutnya hingga iterasi selesai
            // 7. Tulis retcode FS_SUCCESS pada akhir proses pembacaan.

            for (i = 0; i < 16; i++) {
                if (sector_entry_buffer.sector_numbers[i] != 0x0) {
                    readSector(metadata->buffer + (i*512), sector_entry_buffer.sector_numbers[i]);
                } else {
                    break;
                }
            }

            metadata->filesize = i * 512;
            *return_code = FS_SUCCESS;
        }
    }
}
         
void write(struct file_metadata *metadata, enum fs_retcode *return_code) {
    
    struct node_filesystem   node_fs_buffer;
    struct sector_filesystem sector_fs_buffer;
    struct map_filesystem    map_fs_buffer;
	struct node_entry        node_buffer;
	struct sector_entry      sector_entry_buffer;
    int                      i, j, available;
    int                      node_idx, sector_idx;
    bool                     found;

    // Tambahkan tipe data yang dibutuhkan
    
	// Pembacaan storage ke buffer
	readSector(&sector_fs_buffer, FS_SECTOR_SECTOR_NUMBER); 
	readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER); 
	readSector(&(node_fs_buffer.nodes[0]),   FS_NODE_SECTOR_NUMBER);        
	readSector(&(node_fs_buffer.nodes[32]) , FS_NODE_SECTOR_NUMBER + 1);    
    

    // Masukkan filesystem dari storage ke memori

    // 1. Cari node dengan nama dan lokasi parent yang sama pada node.
    //    Jika tidak ditemukan kecocokan, lakukan proses ke-2.
    //    Jika ditemukan node yang cocok, tuliskan retcode 
    //    FS_W_FILE_ALREADY_EXIST dan keluar. 

    i = 0;
    while (i < 64) {
		if((node_fs_buffer.nodes[i].parent_node_index == metadata->parent_index) && (strcmp(node_fs_buffer.nodes[i].name, metadata->node_name))){
            *return_code = FS_W_FILE_ALREADY_EXIST;
            return;
        } else {
            
            i++;
        }
    }
    // 2. Cari entri kosong pada filesystem node dan simpan indeks.
    //    Jika ada entry kosong, simpan indeks untuk penulisan.
    //    Jika tidak ada entry kosong, tuliskan FS_W_MAXIMUM_NODE_ENTRY
    //    dan keluar.
    
    i = 0;
    found = false;
    while (i < 64 && !found) {        
		if((strlen(node_fs_buffer.nodes[i].name) == 0)){
            found = true;
            node_idx = i;
        } else {
            i++;
        }
    }
    
    if (!found) {
        // printString("Nodes Penuh sekaleeeeeeeeeeee\r\n");
        *return_code = FS_W_MAXIMUM_NODE_ENTRY;
        return;
    }

    // 3. Cek dan pastikan entry node pada indeks P adalah folder.
    //    Jika pada indeks tersebut adalah file atau entri kosong,
    //    Tuliskan retcode FS_W_INVALID_FOLDER dan keluar.
    if (node_fs_buffer.nodes[metadata->parent_index].sector_entry_index != FS_NODE_S_IDX_FOLDER && metadata->parent_index != FS_NODE_P_IDX_ROOT){
        *return_code = FS_W_INVALID_FOLDER;
        return;
    }

    // 4. Dengan informasi metadata filesize, hitung sektor-sektor 
    //    yang masih kosong pada filesystem map. Setiap byte map mewakili 
    //    satu sektor sehingga setiap byte mewakili 512 bytes pada storage.
    //    Jika empty space tidak memenuhi, tuliskan retcode
    //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
    //    Jika ukuran filesize melebihi 8192 bytes, tuliskan retcode
    //    FS_W_NOT_ENOUGH_STORAGE dan keluar.
    //    Jika tersedia empty space, lanjutkan langkah ke-5.
    
    if (metadata->filesize > 8192) {
        *return_code = FS_W_NOT_ENOUGH_STORAGE;
        return;
    } 
    
    available = 0;
    for (i = 16; i < 256; i++){
        if (!map_fs_buffer.is_filled[i]) available++;
    }
    
    // Sector sudah penuh
    // printInt(available*512);
    
    if (available * 512 < metadata->filesize){
        *return_code = FS_W_NOT_ENOUGH_STORAGE;
        return;
    }
    
    // Terdapat sector kosong
    // 5. Cek pada filesystem sector apakah terdapat entry yang masih kosong.
    //    Jika ada entry kosong dan akan menulis file, simpan indeks untuk 
    //    penulisan.
    //    Jika tidak ada entry kosong dan akan menulis file, tuliskan
    //    FS_W_MAXIMUM_SECTOR_ENTRY dan keluar.
    //    Selain kondisi diatas, lanjutkan ke proses penulisan.
    found = false;
    i = 0;
    while (i < 32 && !found) {
        if (sector_fs_buffer.sector_list[i].sector_numbers[0] == 0){
            found = true;
            sector_idx = i;
        }
        else i++;
    }

    // Jika tidak ada entry kosong dan akan menulis file
    if (!found) {
        *return_code = FS_W_MAXIMUM_SECTOR_ENTRY;
        return;
    }
    
    // Penulisan
    // 1. Tuliskan metadata nama dan byte P ke node pada memori buffer
    strcpy(node_fs_buffer.nodes[node_idx].name, metadata->node_name);
    node_fs_buffer.nodes[node_idx].parent_node_index = metadata->parent_index;

        // 2. Jika menulis folder, tuliskan byte S dengan nilai 
        //    FS_NODE_S_IDX_FOLDER dan lompat ke langkah ke-8
    if (metadata->filesize == 0) {
        node_fs_buffer.nodes[node_idx].sector_entry_index = FS_NODE_S_IDX_FOLDER;
    } else {
        // 3. Jika menulis file, tuliskan juga byte S sesuai indeks sector
        node_fs_buffer.nodes[node_idx].sector_entry_index = sector_idx;

        // 4. Persiapkan variabel j = 0 untuk iterator entry sector yang kosong
        j = 0;

        // 5. Persiapkan variabel buffer untuk entry sector kosong
        // byte buffer[16];
        // sector_entry_buffer

        // 6. Lakukan iterasi berikut dengan kondisi perulangan (penulisan belum selesai && i = 0..255)
        found = false;
        i = 0;
        while (i < 256 && !found) {
            //    1. Cek apakah map[i] telah terisi atau tidak
            if (map_fs_buffer.is_filled[i]) {
                //    2. Jika terisi, lanjutkan ke iterasi selanjutnya / continue
                // do nothing
            } else {
                //    3. Tandai map[i] terisi
                map_fs_buffer.is_filled[i] = true;
                //    4. Ubah byte ke-j buffer entri sector dengan i
                sector_entry_buffer.sector_numbers[j] = i;
                //    5. Tambah nilai j dengan 1
                j++;
                //    6. Lakukan writeSector() dengan file pointer buffer pada metadata 
                writeSector(metadata->buffer + ((j-1) * 512), i);
                // writeSector(metadata->buffer, i);
                //       dan sektor tujuan i
                //    7. Jika ukuran file yang telah tertulis lebih besar atau sama dengan
                //       filesize pada metadata, penulisan selesai
                if (j * 512 >= metadata->filesize) found = true;
            }
            i++;
        }
        
        // 7. Lakukan update dengan memcpy() buffer entri sector dengan 
        //    buffer filesystem sector
        memcpy(sector_fs_buffer.sector_list[sector_idx].sector_numbers, sector_entry_buffer.sector_numbers, sizeof(sector_entry_buffer));
    }

    // 8. Lakukan penulisan seluruh filesystem (map, node, sector) ke storage
    //    menggunakan writeSector() pada sektor yang sesuai
    writeSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);
    writeSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    writeSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
    writeSector(&sector_fs_buffer, FS_SECTOR_SECTOR_NUMBER);

    // 9. Kembalikan retcode FS_SUCCESS
    *return_code = FS_SUCCESS;

}

void fillMap() {
  struct map_filesystem map_fs_buffer;
  int i;

  // Load filesystem map
  readSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER);

  // Edit filesystem map disini //

   for (i = 0; i < 16; i++){
       map_fs_buffer.is_filled[i] = true;
   }


   for (i = 256; i < 512; i++){
       map_fs_buffer.is_filled[i] = true;
   }                      

  // Update filesystem map
  writeSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER); 
}

