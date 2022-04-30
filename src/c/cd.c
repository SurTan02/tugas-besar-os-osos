#include "header/cd.h"

int main() {
    struct message msg;
    getMessage(&msg);
    
    changeDirectory(msg.current_directory, msg.arg2);
    
    setMessage(&msg);
    while(true);
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
    
    printReturnCode(arg, FS_R_NODE_NOT_FOUND);
}