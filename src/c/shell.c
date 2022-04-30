#include "header/shell.h"

int main() {
	struct message msg;
	struct file_metadata meta;

	char input_buf[64];
	// char argv[4][16];
	// int argc;
	byte current_dir;

	getMessage(&msg);

	current_dir = msg.current_directory;
	
	while (true){
		puts("OS@IF2230:");
	// printCWD
		puts("$ ");
		gets(input_buf);


		// parser(input_buf, output1, output2);
		// getArgument(output1, &argc, (char *)argv, 4, 16);
		

		// strcpy(msg.arg1, argv[0]);
		// strcpy(msg.arg2, argv[1]);
		// strcpy(msg.arg3, argv[2]);
		// strcpy(msg.other, output2);
		
		// msg.argc = argc;
		// setMessage(&msg);
		processArgument(&msg, input_buf);
		
		strcpy(meta.node_name, msg.arg1);
		
		
		meta.parent_index = 0;
		
		executeProgram(&meta, msg.next_program_segment);
	}
	
}
