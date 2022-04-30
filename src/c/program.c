#include "header/program.h"

void executeProgram(struct file_metadata *metadata, int segment){
    interrupt(0x21, 0x06, metadata, segment, 0);
}

void exits(){
    struct file_metadata meta;
    meta.node_name    = "shell";    
    meta.parent_index = 0;
    executeProgram(&meta, 0x2000);
}