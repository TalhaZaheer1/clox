#include "./inputs.h"
#include "./VM/vm.h"

void runPrompt() {
  char input[1024];
  for (;;) {
    printf(">");
    fgets(input, sizeof(input), stdin);
    printf("%s", input);
    interpret(input);
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
  long int fileSize = ftell(sourceFile);
  if (fileSize == -1L) {
    perror("Error file position.");
    exit(1);
  }
  rewind(sourceFile);
  char source[fileSize + 1];
  size_t chrsRead = fread(source, 1, fileSize, sourceFile);
  if (chrsRead < fileSize) {
    perror("Error reading file.");
    exit(1);
  }
  source[fileSize + 1] = '\0';
  interpret(source);
}
