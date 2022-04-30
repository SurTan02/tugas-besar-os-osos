#include "header/message.h"

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
	readSector(msg, 0x104);
}