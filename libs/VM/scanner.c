#include "scanner.h"

Scanner scanner;

void initScanner(char *source) {
  scanner.start = source;
  scanner.current = source;
}
