#include "header/shell.h"

int main() {
  struct message msg;
  struct file_metadata meta;

  char input_buf[64], output1[64], output2[64];
  char argv[4][16];
  int argc;
  byte current_dir;

  getMessage(&msg);

  current_dir = msg.current_directory;
 
  puts("OS@IF2230:");
  // printCWD
  puts("$ ");
  gets(input_buf);

  while (true) {
    gets(input_buf);
    parser(input_buf, output1, output2);
    puts(output1); puts("\r\n");
    puts(output2); puts("\r\n");
  }

  getArgument(input_buf, &argc, (char *)argv, 4, 16);

  strcpy(msg.arg1, argv[0]);
  strcpy(msg.arg2, argv[1]);
  strcpy(msg.arg3, argv[2]);
  strcpy(msg.other, argv[4]);

  msg.argc = argc;
  setMessage(&msg);

  strcpy(meta.node_name, argv[0]);
  meta.parent_index = 0;
  executeProgram(&meta, 3000);
}