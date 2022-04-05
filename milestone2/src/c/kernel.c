// Kode kernel
// PENTING : FUNGSI PERTAMA YANG DIDEFINISIKAN ADALAH main(),
//   cek spesifikasi untuk informasi lebih lanjut

// TODO : Tambahkan implementasi kode C

#include "header/kernel.h"
#include "header/filesystem.h"

int main() {
	fillMap();
	
    makeInterrupt21();
    clearScreen();

	printString("  ______        _______.  ______        _______.	\r\n");
	printString(" /  __  \\      /       | /  __  \\      /       | \r\n");
	printString("|  |  |  |    |   (----`|  |  |  |    |   (----`	\r\n");
	printString("|  |  |  |     \\   \\    |  |  |  |    \\   \\   	\r\n");
	printString("|  `--'  | .----)   |   |  `--'  | .----)   |   	\r\n");
	printString(" \\______/  |_______/     \\______/  |_______/    	\r\n");

    // liveshare: https://prod.liveshare.vsengsaas.visualstudio.com/join?3374F66975C6DC5A7678010381EA56F659AF
    // why?? because of reasons, tyvm
    // gk bisa copy dari vb ke windows D:

	while (true) 
	{
        shell();
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
        if (*string == '\n') interrupt (0x10, 0x0e * 256 + '\r', 0, 0, 0);	

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
	struct node_entry        node_buffer;
	struct sector_filesystem sector_fs_buffer;
	struct sector_entry      sector_entry_buffer;
    int    i;
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
		if((node_fs_buffer.nodes[i].parent_node_index == metadata->parent_index) && (strcmp(node_fs_buffer.nodes[i].name, metadata->node_name))){
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
            // memcpy(&node_buffer, &(node_fs_buffer.nodes[i]), sizeof(node_buffer));
            memcpy(&sector_entry_buffer,&sector_fs_buffer.sector_list[node_fs_buffer.nodes[i].sector_entry_index], sizeof(sector_entry_buffer));

            // 2. Lakukan iterasi proses berikut, i = 0..15
            // 3. Baca byte entry sector untuk mendapatkan sector number partisi file
            // 4. Jika byte bernilai 0, selesaikan iterasi
            // 5. Jika byte valid, lakukan readSector() 
            //    dan masukkan kedalam buffer yang disediakan pada metadata
            // 6. Lompat ke iterasi selanjutnya hingga iterasi selesai
            // 7. Tulis retcode FS_SUCCESS pada akhir proses pembacaan.
            i = 0;
            strcpy(metadata->buffer, buf);
            while (sector_entry_buffer.sector_numbers[i] != 0x0 && i < 16) {
                readSector(buf+i*512, sector_entry_buffer.sector_numbers[i]);
                i++;
            }
            strcpy(metadata->buffer, buf);
            clear(buf, i*512);
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

void list(byte current_dir, int argc, char* arg){
    struct node_filesystem  node_fs_buffer;
    int i, j;
    bool found;

    readSector(&(node_fs_buffer.nodes[0]),   FS_NODE_SECTOR_NUMBER);        
	readSector(&(node_fs_buffer.nodes[32]),  FS_NODE_SECTOR_NUMBER + 1);

    j = 0;
    if (argc == 2) {
        i = 0;
        found = false;
        while (i < 64 && !found) {
            if (node_fs_buffer.nodes[i].parent_node_index = current_dir && 
            strcmp(node_fs_buffer.nodes[i].name, arg) &&
            node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER) {
                found = true;
            } else {
                i++;
            }
        }

        if (!found) {
            printString("Folder not exist\r\n");
            return;
        }
    } else {
        j = current_dir;
    }   

    for (i = 0 ; i < 64 ; i++) {
        if (node_fs_buffer.nodes[i].parent_node_index == j && !strcmp(node_fs_buffer.nodes[i].name,"")){
            printString(node_fs_buffer.nodes[i].name);
            if (node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER) {
                printString("/");
            }
            printString("\r\n");
        }
    }
}

void changeDirectory(char *current_dir, char* arg) {
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
    struct file_metadata*   metadata;
    enum   fs_retcode       return_code;
    int    i;

    strcpy(metadata->node_name, arg);
    metadata->parent_index = current_dir;
    metadata->filesize = 0;

    write(metadata, &return_code);
    if (return_code != FS_SUCCESS){
        printReturnCode(arg, return_code);
    }else{
        clear(metadata->buffer, div(metadata->filesize, 512));
        clear(metadata->node_name, strlen(metadata->node_name));
    }
}

void cat(byte current_dir, char* arg2){
    struct file_metadata*   src;
    enum   fs_retcode       return_code;
    
    strcpy(src->node_name, arg2);
    src->parent_index = current_dir;
    src->filesize = 0;

    read(src, &return_code);
    if (return_code != FS_SUCCESS){
        printReturnCode(arg2, return_code);
    }
    else{
        printFileContent(src->buffer);
        printString("\r\n");
    }

    clear(src->buffer, div(src->filesize, 512));
}

void cp(byte current_dir, char* src, char* dest) {
    struct file_metadata    *srcFile, *dstFile;
    enum   fs_retcode       return_code;
    int    i;
    // char buf[8192];

    strcpy(srcFile->node_name, src);
    srcFile->parent_index = current_dir;
    srcFile->filesize = 0;

    read(srcFile, &return_code);
    if (return_code != FS_SUCCESS){
        printReturnCode(src, return_code);
    } else {
        strcpy(dstFile->node_name, dest);
        dstFile->parent_index = srcFile->parent_index;
        dstFile->filesize = srcFile->filesize;
        strcpy(dstFile->buffer, srcFile->buffer);

        write(dstFile, &return_code);
        if (return_code != FS_SUCCESS){
            printReturnCode(src, return_code);
        }
        clear(dstFile->buffer, div(dstFile->filesize, 512));
    }

    clear(srcFile->buffer, div(srcFile->filesize, 512));
}

void move(byte current_dir, char* src, char* dst) {
    struct node_filesystem  node_fs_buffer;
    int i, idxSrc, parentIdx;
    bool found, isFolderSrc, isFolderDst;
    char name[14];

    readSector(&(node_fs_buffer.nodes[0]),   FS_NODE_SECTOR_NUMBER);
	readSector(&(node_fs_buffer.nodes[32]),  FS_NODE_SECTOR_NUMBER + 1);
    
    // cari indeks src
    found = false;
    i = 0;
    while (i < 64 && !found) {
        if (node_fs_buffer.nodes[i].parent_node_index == current_dir && 
        strcmp(node_fs_buffer.nodes[i].name, src)) {
            found = true;
            idxSrc = i;
        } else {
            i++;
        }
    }

    if (!found) {
        printReturnCode(*src,  FS_R_NODE_NOT_FOUND);
        return;
    }

    isFolderSrc = node_fs_buffer.nodes[idxSrc].sector_entry_index == FS_NODE_S_IDX_FOLDER;

    if (dst[0] == '/') {
        // pindahin ke root

        // Dapetin nama
        strcpy(name, dst+1);

        if (strcmp(name, "")) {
            printString("Nama tidak valid\r\n");
            return;
        }
        
        // cek apakah ada file atau folder dengan nama yang sama di root
        found = false;
        i = 0;
        while (i < 64 && !found) {
            isFolderDst = node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER;
            if (node_fs_buffer.nodes[i].parent_node_index == FS_NODE_P_IDX_ROOT && 
            strcmp(node_fs_buffer.nodes[i].name, name) && 
            isFolderDst == isFolderSrc) {
                found = true;
            } else {
                i++;
            }
        }

        if (found) {
            printReturnCode(src, FS_W_FILE_ALREADY_EXIST);
            return;
        }

        // change parent index to root index
        node_fs_buffer.nodes[idxSrc].parent_node_index = FS_NODE_P_IDX_ROOT;

        // change name
        strcpy(node_fs_buffer.nodes[idxSrc].name, name); 
        printString("mv berhasil\r\n");
    } else if (dst[0] == '.' && dst[1] == '.' && dst[2] == '/') {
        // pindahin mundur

        // Dapetin nama
        strcpy(name, dst+3);

        if (strcmp(name, "")) {
            printString("Nama tidak valid\r\n");
            return;
        }

        // cek apakah ada file atau folder dengan nama yang sama di direktori
        parentIdx = node_fs_buffer.nodes[current_dir].parent_node_index;

        found = false;
        i = 0;
        while (i < 64 && !found) {
            isFolderDst = node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER;
            if (node_fs_buffer.nodes[i].parent_node_index == parentIdx && 
            strcmp(node_fs_buffer.nodes[i].name, name) && 
            isFolderDst == isFolderSrc) {
                found = true;
            } else {
                i++;
            }
        }

        if (found) {
            printReturnCode(src, FS_W_FILE_ALREADY_EXIST);
            return;
        }

        // change parent index to root index
        node_fs_buffer.nodes[idxSrc].parent_node_index = parentIdx;

        // change name
        strcpy(node_fs_buffer.nodes[idxSrc].name, name); 
    } else {
        // Pindahin maju ato langsung rename

        // Dapetin nama
        strcpy(name, dst);

        // cari destinasi
        found = false;
        i = 0;
        while (i < 64 && !found) {
            if (node_fs_buffer.nodes[i].parent_node_index == current_dir && 
            strcmp(node_fs_buffer.nodes[i].name, name)) {
                found = true;
            } else {
                i++;
            }
        }

        if (found) {
            isFolderDst = node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER;
            // kalo misalkan arg3 ternyata ada dan merupakan foler"
            // dia pindahin arg2 ke dalam
            if (isFolderDst) {
                // change parent index
                node_fs_buffer.nodes[idxSrc].parent_node_index = i;
            } else {
                // kala arg3 ada , file
                // tolak
                printReturnCode(src, FS_W_FILE_ALREADY_EXIST);
                return;
            }
        } else {
            // kalo arg3 tidak ada
            // change name
            strcpy(node_fs_buffer.nodes[idxSrc].name, name);
        }
    }

    writeSector(&(node_fs_buffer.nodes[0]), FS_NODE_SECTOR_NUMBER);
    writeSector(&(node_fs_buffer.nodes[32]), FS_NODE_SECTOR_NUMBER + 1);
}

void shell() {
    // IN PROGRESS
    char input_buf[64];
    char path_str[1];
    char argv[4][16];
    int argc;
    byte current_dir = FS_NODE_P_IDX_ROOT;

    
    while (true) {
        printString("OS@IF2230:");
        printCWD(path_str, current_dir);
        printString("$ ");
        readString(input_buf);  

        getArgument(input_buf, &argc, (char *)argv, 4, 16);

        if (strcmp(argv[0], "ls")) {
            if (argc > 2) printString("Too many arguments\r\n");
            else list(current_dir, argc, argv[1]); 

        } else if (strcmp(argv[0], "cd")) {
            if (argc == 2) changeDirectory(&(current_dir), argv[1]);
            else if (argc == 1) printString("Too few arguments\r\n");
            else printString("Too many arguments\r\n");

        } else if (strcmp(argv[0], "mv")) {
            if (argc == 3) move(current_dir, argv[1], argv[2]);
            else if (argc < 3) printString("Too few arguments\r\n");
            else printString("Too many arguments\r\n");

        } else if (strcmp(argv[0], "mkdir")) {
            if (argc == 2) makeDirectory(current_dir, argv[1]);
            else if (argc == 1) printString("Too few arguments\r\n");
            else printString("Too many arguments\r\n");

        } else if (strcmp(argv[0], "cat")) {
            if (argc == 2) cat(current_dir, argv[1]);
            else if (argc == 1) printString("Too few arguments\r\n");
            else printString("Too many arguments\r\n");

        } else if (strcmp(argv[0], "cp")) {
            cp(current_dir, argv[1], argv[2]);

        } else {
            printString("Unknown command\r\n");
        }

        clear(input_buf, strlen(input_buf));
        clear(argv, 4 * 16);
    }
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

void getArgument(char* input_buf, int *argc, char* argv, int maxArg, int size) {
    int i, j;

    i = 0; j = 0; (*argc) = 0;
    while (input_buf[i] != '\0' && (*argc) < maxArg - 1) {
        if (input_buf[i] == ' ') {
            argv[(*argc) * size + j] = '\0';
            j = 0; (*argc)++;
        } else {
            argv[(*argc) * size + j] = input_buf[i];
            j++;
        }
        i++;
    }

    while (input_buf[i] != '\0') {
        argv[(*argc) * size + j] = input_buf[i];
        i++; j++;
    }

    argv[(*argc) * size + j] = '\0';

    (*argc)++;
}

void printReturnCode(char *arg, enum fs_retcode return_code){
    switch (return_code)
    {
    case FS_R_NODE_NOT_FOUND:
        printString("No such file or directory\r\n");
        break;
    case FS_R_TYPE_IS_FOLDER:
        printString(arg);
        printString(" is a directory\r\n");
        break;

    case FS_W_FILE_ALREADY_EXIST:
        printString("File already exist\r\n");
        break;

    case FS_W_NOT_ENOUGH_STORAGE:
        printString("Not Enough storage\r\n");
        break;

    case FS_W_MAXIMUM_NODE_ENTRY:
        printString("Maximum Node Entry\r\n");
        break;

    case FS_W_MAXIMUM_SECTOR_ENTRY:
        printString("Maximum Sector Entry\r\n");
        break;

     case FS_W_INVALID_FOLDER:
        printString("Invalid Folder\r\n");
        break;

    default:
        break;
    }
}