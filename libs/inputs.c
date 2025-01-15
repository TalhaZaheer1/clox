#include "./inputs.h"
#include <stdio.h>

void runPrompt() {
  char input[1024];
  for (;;) {
    printf(">");
    scanf("%s", input);
    run(input);
    printf("\n");
  }
}

void runFile(char *path) {
  FILE *sourceFile = fopen(path, "r");
  if (sourceFile == NULL) {
    perror("Invalid file path: %s");
    exit(1);
  }
}
