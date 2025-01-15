#include "../libs/VM/vm.h"
#include "../libs/representation/chunk.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    runPrompt();
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    perror("Too many arguments.");
    exit(1);
  }
  return 0;
}
