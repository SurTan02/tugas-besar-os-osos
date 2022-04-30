#ifndef MESSAGE_H
#define MESSAGE_H

#include "std_datatype.h"
#include "filesystem.h"
#include "string.h"

struct message {
    byte current_directory;
    char arg1[64];
    char arg2[64];
    char arg3[64];
    int argc;
    int next_program_segment; // Dapat digunakan untuk bagian 3.5
    byte other[311];
    // byte other[317];
};


// void setMessage(byte *arg1,byte *arg2, byte *arg3 ,byte curr_directory);
void setMessage(struct message *msg);

void getMessage(struct message *msg);
#endif