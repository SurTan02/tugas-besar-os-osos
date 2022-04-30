#include "header/mkdir.h"

int main() {
    struct message msg;
    struct file_metadata meta;
    getMessage(&msg);

    makeDirectory(msg.current_directory, msg.arg2);
    
    if (strlen(msg.other) != 0){
        processArgument(&msg, msg.other);
        strcpy(meta.node_name, msg.arg1);
		meta.parent_index = 0;
        
		executeProgram(&meta, msg.next_program_segment);
    } else{
        exits();
    }
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
        clear(metadata->buffer, metadata->filesize);
        clear(metadata->node_name, strlen(metadata->node_name));
    }
}