#include "./vm.h"

VM vm;

void initVM() {}

void freeVM() {}

InterpretResult interpret(Chunk *chunk) {
  vm.chunk = chunk;
  vm.ip = chunk->code;
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (chunk->constantArr.values[READ_BYTE()])
  uint8_t instruction;
  switch (instruction = READ_BYTE()) {
  case OP_RETURN:
    return RETURN_OK;
  case OP_CONSTANT:
    printValue()
  }
}
