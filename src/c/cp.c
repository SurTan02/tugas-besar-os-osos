#include "header/cp.h"

int main() {
    struct message msg;
    struct file_metadata meta;
    getMessage(&msg);

    if (msg.argc == 3) cp(msg.current_directory, msg.arg2, msg.arg3);
    else if (msg.argc < 3) printReturnCode (msg.arg1, ARG_TOO_FEW);
    else printReturnCode (msg.arg1, ARG_TOO_MANY);  
    
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