#include "header/mv.h"

int main() {
    struct message msg;
    struct file_metadata meta;
    
    getMessage(&msg);

    if (msg.argc == 3) move(msg.current_directory, msg.arg2, msg.arg3);
    else if (msg.argc < 3) printReturnCode ("mv", ARG_TOO_FEW);
    else printReturnCode ("mv", ARG_TOO_MANY);
    
    setMessage(&msg);
    if (strlen(msg.other) != 0){
        processArgument(&msg, msg.other);
        
        if (msg.arg1[0] == '.' && msg.arg1[1] == '/') {
			substr(meta.node_name, msg.arg1, 2);
			meta.parent_index = msg.current_directory;

	    } else {
		    meta.parent_index = 0;
		    strcpy(meta.node_name, msg.arg1);
	    }
        
		executeProgram(&meta, msg.next_program_segment);
    } else{
        exits();
    }
}

void move(byte current_dir, char* src, char* dst) {
    struct node_filesystem  node_fs_buffer;
    int i, idxSrc, parentIdx;
    bool found, isFolderSrc, isFolderDst;
    char name[14];

    readSector(&(node_fs_buffer.nodes[0]),   FS_NODE_SECTOR_NUMBER, 0xFF);
	readSector(&(node_fs_buffer.nodes[32]),  FS_NODE_SECTOR_NUMBER + 1, 0xFF);
    
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
            printReturnCode(name, FS_R_NODE_NOT_FOUND);
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
        
    } else if (dst[0] == '.' && dst[1] == '.' && dst[2] == '/') {
        // pindahin mundur

        // Dapetin nama
        strcpy(name, dst+3);

        if (strcmp(name, "")) {
            printReturnCode(name, FS_R_NODE_NOT_FOUND);
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