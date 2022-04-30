#include "header/textio.h"

int puts(char *string) {
  interrupt(0x21, 0x0, string, 0, 0);
}

int gets(char *string) {
  interrupt(0x21, 0x1, string, 0, 0);
}

void getArgument(char* input_buf, int *argc, char* argv, int maxArg, int size) {
  int i, j;

  i = 0; j = 0; (*argc) = 0;
  while (input_buf[i] != '\0' && (*argc) < maxArg - 1) {
      if (input_buf[i] == ' ') {
          argv[(*argc) * size + j] = '\0';
          j = 0; (*argc)++;
      } else {
          argv[(*argc) * size + j] = input_buf[i];
          j++;
      }
      i++;
  }

  while (input_buf[i] != '\0') {
      argv[(*argc) * size + j] = input_buf[i];
      i++; j++;
  }

  argv[(*argc) * size + j] = '\0';

  (*argc)++;
}

void parser(char* input_buf, char* output1, char* output2) {
  int i, j;
  bool kondisi_stop;
  
  i = 0;
  kondisi_stop = input_buf[0] != ' ' && input_buf[1] != ';' && input_buf[2] != ' ';
  while (input_buf[i] != '\0') {
    output1[i] = input_buf[i];
    kondisi_stop = input_buf[i] != ' ' && input_buf[i + 1] != ';' && input_buf[i + 2] != ' ';
    i++;
  }
  output1[i] = '\0';

  if (kondisi_stop) {
    j = 0; i += 3;
    while (input_buf[i] != '\0') {
      output2[j] = input_buf[i];
      i++; j++;
    }
    output2[j] = '\0';
  }
}
