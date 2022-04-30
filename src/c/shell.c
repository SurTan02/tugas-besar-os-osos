#include "header/shell.h"

int main() {
  char input_buf[64];
  char arg1[64];
  char arg2[64];
  char arg3[64];
  struct message msg;
  struct file_metadata meta;
  strcpy(msg.arg1, "1");
  strcpy(msg.arg2, "arg ledua");
  strcpy(msg.arg3, "arg ketiga");
  msg.current_directory = 0xFF;
  *msg.other = input_buf;
  msg.next_program_segment = 5;
  while (true){
    // setMessage(&msg, "ls");
    setMessage(&msg, "ls");
    
    // meta.node_name    = "ls";
    strcpy(meta.node_name, "ls");
    // puts("SEBELUM EXEC ");
    // puts(msg.arg1);
    // puts("\r\n");
    meta.parent_index = 0;
    executeProgram(&meta, 0x3000);
    
    gets(input_buf);
    

    // setMessage(&input_buf, 0xFF);
    
    
    // getMessage(&msg);
    
    // puts(msg.arg1);
    // puts("\n\r");
    // puts(msg.arg2);
    // puts("\n\r");
    // puts(msg.arg3);
    // // puts(input_buf);
    // puts("\n\r");
    // gets(input_buf);
  }
  
  // struct message* msg;
  // struct file_metadata meta;
  // strcpy(msg->arg1, "1");
  // strcpy(msg->arg2, "arg ledua");
  // strcpy(msg->arg3, "arg ketiga");
  // msg->current_directory = 0xFF;
  // // *msg.other = input_buf;
  // msg->next_program_segment = 5;
  // while (true){
  //   setMessage(msg, "ls");
    
  //   // meta.node_name    = "ls";
  //   strcpy(meta.node_name, msg->arg1);
  //   puts("SEBELUM EXEC ");
  //   puts(msg->arg1);
  //   puts("\r\n");
  //   meta.parent_index = 0;
  //   executeProgram(&meta, 0x3000);
    
  //   gets(input_buf);
    

  //   // setMessage(&input_buf, 0xFF);
    
    
  //   // getMessage(&msg);
    
  //   // puts(msg.arg1);
  //   // puts("\n\r");
  //   // puts(msg.arg2);
  //   // puts("\n\r");
  //   // puts(msg.arg3);
  //   // // puts(input_buf);
  //   // puts("\n\r");
  //   // gets(input_buf);
  // }
  
}