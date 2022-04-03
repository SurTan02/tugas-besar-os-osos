// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"
#include "header/filesystem.h"

int main(){
    char buf[128];
	fillMap();
	
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
        shell();
    	// readString(buf);
    	// printString(buf);
		// printString("\r\n");

		// if (strcmp(buf, "clear")){
		// 	break;
		// }
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

void read(struct file_metadata *metadata, enum fs_retcode *return_code) {
	struct node_filesystem   node_fs_buffer;
	struct node_entry        node_buffer;
	struct sector_filesystem sector_fs_buffer;
	struct sector_entry      sector_entry_buffer;
    int    i, parent;
    bool   found;
    char   buf[8192];
    
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
            metadata->buffer = buf;
            while (sector_entry_buffer.sector_numbers[i] != 0x0 && i < 16) {
                readSector(metadata->buffer, i);
                i++;
            }
            metadata->filesize = i;
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
    int                      i, parent, j;
    int                      node_idx, sector_idx;
    bool                     found, flag;
    // byte                     buffer[16];
    // Tambahkan tipe data yang dibutuhkan
    
	// Pembacaan storage ke buffer
	readSector(&sector_fs_buffer, FS_SECTOR_SECTOR_NUMBER); 
	readSector(&(node_fs_buffer.nodes[0]),   FS_NODE_SECTOR_NUMBER);        //directory
	readSector(&(node_fs_buffer.nodes[32]) , FS_NODE_SECTOR_NUMBER + 1);    //FILES
    
    // Masukkan filesystem dari storage ke memori

    // 1. Cari node dengan nama dan lokasi parent yang sama pada node.
    //    Jika tidak ditemukan kecocokan, lakukan proses ke-2.
    //    Jika ditemukan node yang cocok, tuliskan retcode 
    //    FS_W_FILE_ALREADY_EXIST dan keluar. 

    i = 0;
    while (i < 64) {
		parent = node_fs_buffer.nodes[i].parent_node_index;
		if((parent == metadata->parent_index) && (strcmp(node_fs_buffer.nodes[i].name, metadata->node_name))){
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
    while (i < 64 && found) {
		if((strlen(node_fs_buffer.nodes[i].name) == 0)){
            found = true;
            return;
        } else {
            i++;
        }
    }

    if (!found) {
        *return_code = FS_W_MAXIMUM_NODE_ENTRY;
        return;
    } else {
        node_idx = i;
    }

    // 3. Cek dan pastikan entry node pada indeks P adalah folder.
    //    Jika pada indeks tersebut adalah file atau entri kosong,
    //    Tuliskan retcode FS_W_INVALID_FOLDER dan keluar.
    
    parent =  metadata->parent_index;
    if (node_fs_buffer.nodes[parent].sector_entry_index != FS_NODE_S_IDX_FOLDER){
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
    
    // File size <= 8192, cari sector yang masih kosong 
    i = 16;
    found = false;
    while  (i < 256 && !found){
        if (!map_fs_buffer.is_filled[i]){
            j = i;
            while ((j < 256) && (j < i + metadata->filesize)) {
                if (!map_fs_buffer.is_filled[j]) break;
                else j++;
            }
            if (j == i + metadata->filesize - 1) found = true;
            else i = j + 1;
        } else {
            i++;
        }
    }

    // Sector sudah penuh
    if (!found) {
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
        if (sector_fs_buffer.sector_list[i].sector_numbers[0] == 0) found = true;
        else i++;
    }

    // Jika tidak ada entry kosong dan akan menulis file
    if (!found) {
        *return_code = FS_W_MAXIMUM_SECTOR_ENTRY;
        return;
    } else {
        sector_idx = i;
    }
    
    // Penulisan
    // 1. Tuliskan metadata nama dan byte P ke node pada memori buffer
    strcpy(node_fs_buffer.nodes[node_idx].name, metadata->node_name);
    node_fs_buffer.nodes[node_idx].parent_node_index = metadata->parent_index;

        // 2. Jika menulis folder, tuliskan byte S dengan nilai 
    if (metadata->filesize == 0) {
        node_fs_buffer.nodes[node_idx].sector_entry_index = FS_NODE_S_IDX_FOLDER;
        //    FS_NODE_S_IDX_FOLDER dan lompat ke langkah ke-8
    } else {
        // 3. Jika menulis file, tuliskan juga byte S sesuai indeks sector
        node_fs_buffer.nodes[node_idx].sector_entry_index = sector_idx;

        // 4. Persiapkan variabel j = 0 untuk iterator entry sector yang kosong
        j = 0;

        // 5. Persiapkan variabel buffer untuk entry sector kosong
        // byte buffer[16];
        // sector_entry_buffer

        // 6. Lakukan iterasi berikut dengan kondisi perulangan (penulisan belum selesai && i = 0..255)
        flag = false;
        i = 0;
        while (i < 256 && !flag) {
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
                //       dan sektor tujuan i
                writeSector(metadata->buffer, i);
                //    7. Jika ukuran file yang telah tertulis lebih besar atau sama dengan
                //       filesize pada metadata, penulisan selesai
                if (j * 512 >= metadata->filesize) flag = true;
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
    writeSector(&node_fs_buffer, FS_NODE_SECTOR_NUMBER);
    writeSector(&sector_fs_buffer, FS_SECTOR_SECTOR_NUMBER);

    // 9. Kembalikan retcode FS_SUCCESS
    *return_code = FS_SUCCESS;
}

void printRec(char* path_str, byte current_dir) {
    struct node_filesystem node_fs_buffer;

    readSector(&(node_fs_buffer.nodes[0]),   FS_NODE_SECTOR_NUMBER);        
	readSector(&(node_fs_buffer.nodes[32]),  FS_NODE_SECTOR_NUMBER + 1);    

    if (node_fs_buffer.nodes[current_dir].parent_node_index == FS_NODE_P_IDX_ROOT) {
        printString(node_fs_buffer.nodes[current_dir].name);
        return;
    } else {
        printRec(path_str, node_fs_buffer.nodes[current_dir].parent_node_index);
        printString("/");
        printString(node_fs_buffer.nodes[current_dir].name);
    }

}

void printCWD(char* path_str, byte current_dir) {
    struct node_filesystem node_fs_buffer;
    int index[64];
    int i,idx;

    readSector(&(node_fs_buffer.nodes[0]),   FS_NODE_SECTOR_NUMBER);        
	readSector(&(node_fs_buffer.nodes[32]),  FS_NODE_SECTOR_NUMBER + 1);    

    printString("~");
    if (current_dir == FS_NODE_P_IDX_ROOT) { 
        // do nothing
    } else {
        printRec(path_str, current_dir);
    }
}

void list(byte current_dir){
    struct node_filesystem  node_fs_buffer;
    int i;

    readSector(&(node_fs_buffer.nodes[0]),   FS_NODE_SECTOR_NUMBER);        //directory
	readSector(&(node_fs_buffer.nodes[32]),  FS_NODE_SECTOR_NUMBER + 1);    //FILES

    for (i = 0 ; i < 64 ; i++) {
        if (node_fs_buffer.nodes[i].parent_node_index == current_dir && !strcmp(node_fs_buffer.nodes[i].name,"")){
            printString(node_fs_buffer.nodes[i].name);
            if (node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER) {
                printString("/");
            }
            printString("\r\n");
        }
    }
}

void changeDirectory(char *current_dir, char* arg){
    struct node_filesystem  node_fs_buffer;
    int i;

    readSector(&(node_fs_buffer.nodes[0]),   FS_NODE_SECTOR_NUMBER);        //directory
	readSector(&(node_fs_buffer.nodes[32]),  FS_NODE_SECTOR_NUMBER + 1);    //FILES

    if (strcmp(arg, "/")) {
        *current_dir = FS_NODE_P_IDX_ROOT;
        return;
    }
    
    if (strcmp(arg, "..")) {
        if (*current_dir != FS_NODE_P_IDX_ROOT){
            *current_dir = node_fs_buffer.nodes[*current_dir].parent_node_index;
            return;
        }
    }

    for (i = 0; i < 64; i++) {
        if (node_fs_buffer.nodes[i].parent_node_index == *current_dir && 
            strcmp(arg, node_fs_buffer.nodes[i].name) && 
            node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER) {
                *current_dir = i;
                return;
        }
    }
    
    printString("Directory not found!\r\n");
}

void makeDirectory(byte current_dir, char* arg) {
    struct node_filesystem  node_fs_buffer;
    struct file_metadata    metadata;
    enum   fs_retcode       return_code;
    int    i;

    readSector(&(node_fs_buffer.nodes[0]),   FS_NODE_SECTOR_NUMBER);        //directory
	readSector(&(node_fs_buffer.nodes[32]),  FS_NODE_SECTOR_NUMBER + 1);

    printString("TES\r\n");
    printString(node_fs_buffer.nodes[current_dir].name);
    printString("TES SETELAH\r\n");
    // byte *buffer;
    // char *node_name;
    // byte parent_index;
    // unsigned int filesize;

    strcpy(metadata.node_name, arg);
    metadata.parent_index = node_fs_buffer.nodes[current_dir].parent_node_index;
    metadata.filesize = 0;
    write(&metadata, &return_code);
    printString("TES SETELSSS\r\n");
    // for (i = 0; i < 64 && node_fs_buffer.nodes[i] != 0x0; i++) {

    // }
    
    // if (i >= 64) {
    //     printString("Limit sector") 
    //     return;
    // }

    // searchDir();
}

void shell() {
    // IN PROGRESS
    char input_buf[64];
    char path_str[128];
    char arg1[64], arg2[64];
    byte current_dir = FS_NODE_P_IDX_ROOT;

    
    while (true) {
        printString("OS@IF2230:");
        printCWD(path_str, current_dir);
        printString("$ ");
        readString(input_buf);  
        
        split(input_buf, arg1, arg2);

        if (strcmp(arg1, "cd")) {
            changeDirectory(&(current_dir), arg2);
        } else if (strcmp(arg1, "ls")) {
            list(current_dir); 
        } else if (strcmp(arg1, "mv")) {
            // Disini mv
        } else if (strcmp(arg1, "mkdir")) {
            makeDirectory(current_dir, arg2);
        } else if (strcmp(arg1, "cat")) {
            // Disini cat
        } else if (strcmp(arg1, "cp")) {
            // Disini cp
        } else {
            printString("Unknown command\r\n");
        }
    }
}

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


   for (i = 256; i < 512; i++){
       map_fs_buffer.is_filled[i] = true;
   }                      

  // Update filesystem map
  writeSector(&map_fs_buffer, FS_MAP_SECTOR_NUMBER); 
}

// void getArgument(char* input_buf, char* arg1, char* arg2, char* arg3, char* arg4) {
//     int i, j;
    
//     i = 0;
//     while (input_buf[i] != ' ' && input_buf[i] != '\0') {
//         arg1[i] = input_buf[i];
//         i++;
//     }
//     arg1[i] = '\0';

//     j = 0;
//     while (input_buf[i] != ' ' && input_buf[i] != '\0') {
//         arg2[j] = input_buf[i];
//         i++; j++;
//     }
//     arg2[j] = '\0';

//     j = 0;
//     while (input_buf[i] != ' ' && input_buf[i] != '\0') {
//         arg3[j] = input_buf[i];
//         i++; j++;
//     }
//     arg3[j] = '\0';

//     j = 0;
//     while (input_buf[i] != '\0') {
//         arg4[j] = input_buf[i];
//         i++; j++;
//     }
//     arg4[j] = '\0';
// }

void split(char* input_buf, char* arg1, char* arg2) {
    int i, j;
    
    i = 0;
    while (input_buf[i] != ' ' && input_buf[i] != '\0') {
        arg1[i] = input_buf[i];
        i++;
    }
    arg1[i] = '\0';
    i++;

    j = 0;
    while (input_buf[i] != '\0') {
        arg2[j] = input_buf[i];
        i++;
        j++;
    }
    arg2[j] = '\0';
}