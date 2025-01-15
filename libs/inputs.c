#include "./inputs.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
    perror("Invalid file path: %s.");
    exit(1);
  }
  fseek(sourceFile, 0, SEEK_END);
  long int filePos = ftell(sourceFile);
  if (filePos == -1L) {
    perror("Error file position.");
    exit(1);
  }
  char source[filePos];
  size_t chrsRead = fread(source, 1, filePos, sourceFile);
  if (chrsRead < filePos) {
    perror("Error reading file.");
    exit(1);
  }
  run(source);
}

void run(char *source) { interpret(source); }
