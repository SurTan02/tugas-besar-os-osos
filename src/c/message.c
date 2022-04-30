#include "header/message.h"

void setMessage(struct message *msg){
	writeSector(msg, 0x104);
}

void getMessage(struct message *msg){
	readSector(msg, 0x104);
}