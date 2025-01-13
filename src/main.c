#include "../libs/VM/vm.h"
#include "../libs/representation/chunk.h"

int main(int argc, char *argv[]) {
  Chunk chunk;
  initChunk(&chunk);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, addConstant(&chunk, 77), 123);
  writeChunk(&chunk, OP_RETURN, 123);
  interpret(&chunk);

  freeChunk(&chunk);
  return 0;
}
