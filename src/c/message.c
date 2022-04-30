#include "header/message.h"

// void setMessage(byte *arg1,byte *arg2, byte *arg3 ,byte curr_directory){
// 	struct message test;
// 	struct file_metadata metadata;
// 	enum fs_retcode  retcode;
// 	int i = 0;

// 	test.current_directory = curr_directory;
// 	strcpy(test.arg1, arg1);
// 	strcpy(test.arg2, arg2);
// 	strcyp(test.arg3, arg3);
	
// 	// for (i = 0; i < 319; i++)
// 	// test.other[i] = 0x61;

// 	metadata.buffer       = &test;
// 	metadata.node_name    = "message";
// 	metadata.filesize     = 512;
// 	metadata.parent_index = test.current_directory;
	
// 	write(&metadata, &retcode);
// 	// read(&metadata, &retcode);
// }

void setMessage(struct message *msg){
	
	struct file_metadata *metadata;
	enum fs_retcode  retcode;
	int i = 0;

	// strcpy(msg->arg1, word);

	msg->current_directory = 0xFF;
	msg->next_program_segment = 0xFF;
	
	// for (i = 0; i< 3; i++){
	// 	if (i == 0) strcpy(metadata->buffer + i*512, msg->arg1);
	// 	else if (i == 1) strcpy(metadata->buffer + i*512, msg->arg2);
	// 	else if (i == 2) strcpy(metadata->buffer + i*512, msg->arg3);
	// }

	// metadata->buffer = msg;
	
	// write(metadata, &retcode);
	// read(&metadata, &retcode);
	writeSector(msg, 0x104);
}

void getMessage(struct message *msg){
	struct file_metadata *metadata;
	enum fs_retcode  retcode;
	int i = 0;
	// strcpy(metadata->node_name, "message");

	// read(metadata, &retcode);
	readSector(msg, 0x104);

	// puts(*msg);
	// puts(msg);
	// puts("\r\n");
	// strcpy(msg, metadata->buffer);
	
}

// void setMessage(byte arg1, byte arg2, byte arg3, byte next, int parent_index){
// 	struct node_filesystem   node_fs_buffer;
// }