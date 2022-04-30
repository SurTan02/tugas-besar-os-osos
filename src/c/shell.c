#include "header/shell.h"

int main() {
	char arg1[64];
	char arg2[64];
	char arg3[64];

	char input_buf[64];
    char path_str[1];
    int argc;
    byte current_dir = FS_NODE_P_IDX_ROOT;

	struct message msg;
	struct file_metadata meta;
	

	msg.current_directory = 0xFF;
	
	msg.next_program_segment = 5;

  

  while (true){
    puts("OS@IF2230:");
    // printCWD(path_str, current_dir);
    puts("$ ");
    gets(msg.arg1);  
    gets(msg.arg2);  
    gets(msg.arg3);  
    gets(msg.other);  

    setMessage(&msg);
	// getMessage(&msg);
    // meta.node_name    = "ls";
    
	if (!strcmp(msg.arg1,"")){
		strcpy(meta.node_name, msg.arg1);
		 meta.parent_index = 0;
    	executeProgram(&meta, 0x3000);
	}    
  }
}

void getArgument(char* input_buf, int *argc, char* argv, int maxArg, int size) {
    int i, j;

    i = 0; j = 0; (*argc) = 0;
    while (input_buf[i] != '\0' && (*argc) < maxArg - 1) {
        if (input_buf[i] == ' ') {
            argv[(*argc) * size + j] = '\0';
            j = 0; (*argc)++;
        } else {
            argv[(*argc) * size + j] = input_buf[i];
            j++;
        }
        i++;
    }

    while (input_buf[i] != '\0') {
        argv[(*argc) * size + j] = input_buf[i];
        i++; j++;
    }

    argv[(*argc) * size + j] = '\0';

    (*argc)++;
}

parser(A, A1, A2)
getArgument(A1)
A2 send other


cd -> shell -> ls

//cd include shell
cd -> ls