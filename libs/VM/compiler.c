#include "./compiler.h"
#include "./scanner.h"
#include <stdio.h>

void compile(char *source) {
  initScanner(source);
  int line = -1;
  for (;;) {
    Token token = scanToken();
    if (token.line == line) {
      printf("   | ");
    } else {
      printf("%4d ", token.line);
      line = token.line;
    }
    printf("%2d %.*s \n", token.type, token.length, token.lexeme);

    if (token.type == TOKEN_EOF)
      break;
  }
}
