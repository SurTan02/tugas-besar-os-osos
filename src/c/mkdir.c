#include "header/mkdir.h"

int main() {
    struct message msg;
    struct file_metadata meta;
    getMessage(&msg);

    if (msg.argc == 2) makeDirectory(msg.current_directory, msg.arg2);
    else if (msg.argc == 1) printReturnCode (msg.arg1, ARG_TOO_FEW);
    else printReturnCode (msg.arg1, ARG_TOO_MANY);
    
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