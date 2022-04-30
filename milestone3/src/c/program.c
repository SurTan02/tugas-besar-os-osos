#include "header/program.h"

void executeProgram(struct file_metadata *metadata, int segment){
    // puts(metadata->node_name); puts(" iki nama\r\n");
    bool flag;
    interrupt(0x21, 0x06, metadata, segment, &flag);
    
    if (!flag){
        puts("exec: file not found\r\n");
        exits();
    }
        
    
}

void exits(){
    struct file_metadata meta;
    meta.node_name    = "shell";    
    meta.parent_index = 0;
    executeProgram(&meta, 0x2000);
}