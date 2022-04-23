#include "header/textio.h"

int puts(char *string) {
  interrupt(0x21, 0x0, string, 0, 0);
}

int gets(char *string) {
  interrupt(0x21, 0x1, string, 0, 0);
}