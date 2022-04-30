#include "header/ls.h"

int main() {
    struct message msg;
    getMessage(&msg);
    
    puts("DALAM LS");
    puts(msg.arg1);
    puts("\r\n");
    
    list(msg.current_directory, 1, msg.arg2);
    // exit
}



void list(byte current_dir, int argc, char* arg){
    struct node_filesystem node_fs_buffer;
    int i, j;
    bool found;

    readSector(&(node_fs_buffer.nodes[0]),   FS_NODE_SECTOR_NUMBER, current_dir);        
	readSector(&(node_fs_buffer.nodes[32]),  FS_NODE_SECTOR_NUMBER + 1, current_dir);
    
    
    j = 0;
    if (argc == 2) {
        i = 0;
        found = false;
        while (i < 64 && !found) {
            // = tidak sama dengan ==
            if (node_fs_buffer.nodes[i].parent_node_index == current_dir && 
            strcmp(node_fs_buffer.nodes[i].name, arg) &&
            node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER) {
                found = true;
                j = i;
            } else {
                i++;
            }
        }

        if (!found) {
            // printReturnCode(arg, FS_R_NODE_NOT_FOUND);
            return;
        }
    } else {
        j = current_dir;
    }   
    
    for (i = 0 ; i < 64 ; i++) {
        if (node_fs_buffer.nodes[i].parent_node_index == j && !strcmp(node_fs_buffer.nodes[i].name,"")){
            puts(node_fs_buffer.nodes[i].name);
            if (node_fs_buffer.nodes[i].sector_entry_index == FS_NODE_S_IDX_FOLDER) {
                puts("/");
            }
            puts("\r\n");
        }
    }
}