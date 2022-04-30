#include "header/message.h"

void setMessage(struct message *msg){
	writeSector(msg, 0x104);
}

void getMessage(struct message *msg){
	readSector(msg, 0x104);
}

void processArgument(struct message *msg, char* argument){
	char output1[64], output2[64];
	char argv[4][16];
	int argc;
	
	clear(output1, 64);
	clear(output2, 64);

	// if (getCurrentSegment() == 3000){
	// 	msg->next_program_segment = 4000;
	// }else{
		msg->next_program_segment = getCurrentSegment()  + 1000;
	// }

	// puts(argument); puts("=>argumen\r\n");
	parser(argument, output1, output2);
	// puts(output1); puts("=>o1\r\n");
	// puts(output2); puts("=>o2\r\n");
	getArgument(output1, &argc, (char *)argv, 4, 16);
		

	strcpy(msg->arg1, argv[0]);
	strcpy(msg->arg2, argv[1]);
	strcpy(msg->arg3, argv[2]);
	strcpy(msg->other, output2);
		
	msg->argc = argc;
	setMessage(msg);	
}