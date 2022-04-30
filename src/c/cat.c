#include "header/cat.h"

// buka ls.c
//setiap main() utility, bikin mirip kek ls, OK?
// gk mau

int main() {
    struct message msg;
    getMessage(&msg);
    puts(msg.arg2);
    cat(msg.current_directory, msg.arg2);
    // exit

    exits();
    
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