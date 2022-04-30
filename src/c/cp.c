#include "header/cp.h"

int main() {
    struct message msg;
    struct file_metadata meta;
    getMessage(&msg);

    cp(msg.current_directory, msg.arg2, msg.arg3);
    setMessage(&msg);
    if (strlen(msg.other) != 0){
        processArgument(&msg, msg.other);
        strcpy(meta.node_name, msg.arg1);
		meta.parent_index = 0;
        
		executeProgram(&meta, msg.next_program_segment);
    } else{
        exits();
    }
}

void cp(byte current_dir, char* src, char* dest) {
    struct file_metadata*    srcFile;
    enum   fs_retcode       return_code;
    int    i;

    strcpy(srcFile->node_name, src);
    srcFile->parent_index = current_dir;
    srcFile->filesize = 0;
    
    read(srcFile, &return_code);

    if (return_code != FS_SUCCESS){
        printReturnCode(src, return_code);
    } else {
        strcpy(srcFile->node_name, dest);
        write(srcFile, &return_code);

        if (return_code != FS_SUCCESS){
            printReturnCode(src, return_code);
        }
    }

    // clear(srcFile->buffer, srcFile->filesize);
}