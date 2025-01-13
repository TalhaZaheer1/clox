#include "./vm.h"
#include "../disassembler/disassembler.h"
#include <stdio.h>

#define DEBUG_TRACE_EXECUTION

VM vm;

void initVM() {}

void freeVM() {}

InterpretResult interpret(Chunk *chunk) {
  vm.chunk = chunk;
  vm.ip = chunk->code;

#ifdef DEBUG_TRACE_EXECUTION
  disassembleInstruction(chunk, (int)(vm.ip - chunk->code));

#endif

#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (chunk->constantArr.values[READ_BYTE()])

  uint8_t instruction;
  for (;;) {
    switch (instruction = READ_BYTE()) {
    case OP_RETURN:
      return RETURN_OK;
    case OP_CONSTANT:
      printValue(READ_CONSTANT());
      break;
    default:
      printf("Invalid Instruction.");
      return RETURN_RUNTIME_EXCEPTION;
    }
    printf("\n");
  }

#undef READ_BYTE
#undef READ_CONSTANT
}
