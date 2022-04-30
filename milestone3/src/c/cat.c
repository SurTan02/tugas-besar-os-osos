#include "header/cat.h"

// buka ls.c
//setiap main() utility, bikin mirip kek ls, OK?
// gk mau

int main() {
    struct message msg;
    struct file_metadata meta;
    getMessage(&msg);
    
    if (msg.argc == 2) cat(msg.current_directory, msg.arg2);
    else if (msg.argc == 1) printReturnCode (msg.arg1, ARG_TOO_FEW);
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
        puts("\r\n");
    }

    // clear(src->buffer, src->filesize);
}

// void printFileContent(char *string) {
// 	while (*string != '\0')
// 	{
//         if (*string == '\n') {
//             puts('\r');
//         }
        		
//         puts(string);
//         string++;
// 	}
// }