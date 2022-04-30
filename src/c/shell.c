#include "header/shell.h"

int main() {
	struct message msg;
	struct file_metadata meta;
	char path_str[1];
	char input_buf[64];
	// char argv[4][16];
	// int argc;
	byte current_dir;

	getMessage(&msg);

	current_dir = msg.current_directory;
	
	while (true){
		puts("OS@IF2230:");
		printCWD(path_str, current_dir);
		puts("$ ");
		gets(input_buf);


		processArgument(&msg, input_buf);
		
		strcpy(meta.node_name, msg.arg1);
		
		
		meta.parent_index = 0;
		
		executeProgram(&meta, msg.next_program_segment);
	}
	
}
