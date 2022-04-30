#include "header/ls.h"

int main() {
    struct message msg;
    struct file_metadata meta;
    // int arg1;
    getMessage(&msg);
    
    puts("Masuk main ls\r\n");
    
    puts("arg1: "); puts(msg.arg1); puts("\r\n");
    puts("arg2: "); puts(msg.arg2); puts("\r\n");
    puts("arg3: "); puts(msg.arg3); puts("\r\n");
    
    // arg2 = msg.arg2[0] - '0';
    list(msg.current_directory, 2, msg.arg2);
    // exit
     
    // meta.node_name    = "shell";
    // meta.parent_index = 0;
    // executeProgram(&meta, 0x4000);
    while(true);
}



void list(byte current_dir, int argc, char* arg){
    struct node_filesystem node_fs_buffer;
    int i, j;
    bool found;

    puts("didalam func list\r\n");

    readSector(&(node_fs_buffer.nodes[0]),   FS_NODE_SECTOR_NUMBER);        
	readSector(&(node_fs_buffer.nodes[32]),  FS_NODE_SECTOR_NUMBER + 1);
    
    
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