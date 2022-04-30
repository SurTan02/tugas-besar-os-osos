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

void setMessage(struct message *msg, char *word){
	struct file_metadata *metadata;
	enum fs_retcode  retcode;
	int i = 0;

	strcpy(msg->arg1, word);

	msg->current_directory = 0xFF;
	for (i = 0; i < 64; i++) {
		msg->arg1[i] = 0x41;
		msg->arg2[i] = 0x42;
		msg->arg3[i] = 0x43;
	}
	for (i = 0; i < 317; i++)
		msg->other[i] = 0x61;
	msg->next_program_segment = 0xFF;

	metadata->buffer = &msg;
	
	metadata->node_name    = "message";
	// strcpy(metadata->node_name, "message");
	metadata->filesize     = 512;
	metadata->parent_index = 0xFF;
	
	write(metadata, &retcode);
	// read(&metadata, &retcode);
}

void getMessage(struct message *msg){
	struct file_metadata *metadata;
	enum fs_retcode  retcode;
	int i = 0;

	metadata->parent_index =0xFF;
	metadata->node_name = "message";
	// strcpy(metadata->node_name, "message");

	read(metadata, &retcode);
	puts("nama "); puts(metadata->node_name); puts("\r\n");
	puts("ggag "); puts(metadata->buffer); puts("\r\n");

	strcpy(msg, metadata->buffer);
	// msg = metadata.buffer;
	// strcpy(test.arg1, arg1);
	// strcpy(test.arg2, arg2);
	// strcyp(test.arg3, arg3);
	
	// for (i = 0; i < 319; i++)
	// test.other[i] = 0x61;
}

// argv[1]
// "cp "file1" file2"
// struct message test;
// struct file_metadata metadata;
// struct fs_retcode retcode;
// int i = 0;

// test.current_directory = 0xFF;
//   test.arg1[i] = argv[1];
//   test.arg2[i] = file2;
//   test.arg3[i] = 0x43;

// for (i = 0; i < 319; i++)
//   test.other[i] = 0x61;

// metadata.buffer       = &test;
// metadata.node_name    = "message";
// metadata.filesize     = 512;
// metadata.parent_index = FS_NODE_P_IDX_ROOT;

// write(&metadata, &retcode);

