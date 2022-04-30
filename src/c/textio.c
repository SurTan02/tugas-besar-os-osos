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
  kondisi_stop = input_buf[0] == ' ' && input_buf[1] == ';' && input_buf[2] == ' ';
  while (input_buf[i] != '\0' && !kondisi_stop) {
    output1[i] = input_buf[i];
    i++;
    kondisi_stop = input_buf[i] == ' ' && input_buf[i + 1] == ';' && input_buf[i + 2] == ' ';
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

void printToInt(int x) {
    char string[16];
    char reverse[16];
    int i;
    
    if (x == 0) {
        string[0] = x + '0';
        string[1] = '\0';
    } else {
        i = 0;
        while (x != 0) {
            string[i] = mod(x, 10) + '0';
            x = div(x, 10);
            i++;
        }
        string[i] = '\0';
    }

    for (i = strlen(string) - 1; i > -1; i--) {
        reverse[strlen(string) - 1 - i] = string[i];
    }
    reverse[strlen(string)] = '\0';

    puts(reverse);
}


void printReturnCode(char *arg, enum fs_retcode return_code) {
    
    switch (return_code)
    {
    case FS_R_NODE_NOT_FOUND:
        puts("No such file or directory\r\n");
        break;
    case FS_R_TYPE_IS_FOLDER:
        puts(arg);
        puts(" is a directory\r\n");
        break;

    case FS_W_FILE_ALREADY_EXIST:
        puts("File already exist\r\n");
        break;

    case FS_W_NOT_ENOUGH_STORAGE:
        puts("Not Enough storage\r\n");
        break;

    case FS_W_MAXIMUM_NODE_ENTRY:
        puts("Maximum Node Entry\r\n");
        break;

    case FS_W_MAXIMUM_SECTOR_ENTRY:
        puts("Maximum Sector Entry\r\n");
        break;

    case FS_W_INVALID_FOLDER:
        puts("Invalid Folder\r\n");
        break;

    case ARG_TOO_MANY:
        puts(arg);
        puts(": Too many arguments\r\n");
        break;

    case ARG_TOO_FEW:
        puts(arg);
        puts(": Too few arguments\r\n");
        break;
        
    default:
        break;
    }
}