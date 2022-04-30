#include "header/cat.h"

int main() {
    struct message msg;
    // getMessage();
    cat(msg.current_directory, msg.arg1);
    // exit
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

    // clear(src->buffer, src->filesize);
}