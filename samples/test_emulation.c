#include <stdio.h>

int main(int argc, char **argv) {
  (void) argv;
  if (argc < 5) {
    puts("argc < 5");
  } else {
    puts("argc >= 5");
  }
  printf("end\n");
  return 0;
}