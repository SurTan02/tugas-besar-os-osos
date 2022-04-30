#include "header/cd.h"

int main() {
    struct message msg;
    struct file_metadata meta;
    byte dir;
    
    getMessage(&msg);
    
    dir = msg.current_directory;
    
    if (msg.argc == 2) changeDirectory(&dir, msg.arg2);
    else if (msg.argc == 1) printReturnCode (msg.arg1, ARG_TOO_FEW);
    else printReturnCode (msg.arg1, ARG_TOO_MANY);
    
    
    msg.current_directory = dir;
    setMessage(&msg);
    if (strlen(msg.other) != 0){
        processArgument(&msg, msg.other);

        if (msg.arg1[0] == '.' && msg.arg1[1] == '/') {
			substr(meta.node_name, msg.arg1, 2);
			meta.parent_index = msg.current_directory;

            // puts(meta.node_name);
	    } else {
		    meta.parent_index = 0;
		    strcpy(meta.node_name, msg.arg1);
	    }
        
		executeProgram(&meta, msg.next_program_segment);
    } else{
        exits();
    }
}

void changeDirectory(char *current_dir, char* arg) {
    struct node_filesystem  node_fs_buffer;
    int i;

    readSector(&(node_fs_buffer.nodes[0]),   FS_NODE_SECTOR_NUMBER);        
	readSector(&(node_fs_buffer.nodes[32]),  FS_NODE_SECTOR_NUMBER + 1);    

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