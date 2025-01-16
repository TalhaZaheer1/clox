#include "./vm.h"

#include "./compiler.h"

#define DEBUG_TRACE_EXECUTION

VM vm;

void initVM() { resetStack(); }

void freeVM() {}

void resetStack() { vm.stackTop = vm.stack; }

void push(Value value) {
  if (vm.stackTop - vm.stack > MAX_STACK)
    return;
  *vm.stackTop = value;
  vm.stackTop++;
}

Value pop() {
  if (vm.stackTop == vm.stack)
    return 0;
  return *--vm.stackTop;
}

InterpretResult interpret(char *source) {
  Chunk chunk;
  initChunk(&chunk);

  if (!compile(source, &chunk)) {
    freeChunk(&chunk);
    return INTERPRET_COMPILETIME_EXCEPTION;
  }

  vm.chunk = &chunk;
  vm.ip = chunk.code;
  //
  // #define READ_BYTE() (*vm.ip++)
  // #define READ_CONSTANT() (chunk->constantArr.values[READ_BYTE()])
  // #define BINARY_OP(op) \
//   do { \
//     double b = pop(); \
//     double a = pop(); \
//     push(a op b); \
//   } while (false)
  //
  //   uint8_t instruction;
  //   for (;;) {
  // #ifdef DEBUG_TRACE_EXECUTION
  //     printf("           ");
  //     for (Value *itr = vm.stack; itr < vm.stackTop; itr++) {
  //       printf("[ ");
  //       printValue(*itr);
  //       printf(" ]");
  //     }
  //     printf("\n");
  //     disassembleInstruction(chunk, (int)(vm.ip - chunk->code));
  //
  // #endif
  //
  //     switch (instruction = READ_BYTE()) {
  //
  //     case OP_NEGATE:
  //       push(-pop());
  //       break;
  //     case OP_MULTIPLY:
  //       BINARY_OP(*);
  //       break;
  //     case OP_DIVIDE:
  //       BINARY_OP(/);
  //       break;
  //     case OP_ADD:
  //       BINARY_OP(+);
  //       break;
  //     case OP_SUBSTRACT:
  //       BINARY_OP(-);
  //       break;
  //     case OP_RETURN:
  //       printValue(pop());
  //       printf("\n");
  //       return RETURN_OK;
  //     case OP_CONSTANT:
  //       push(READ_CONSTANT());
  //       break;
  //     default:
  //       printf("Invalid Instruction.");
  //       return RETURN_RUNTIME_EXCEPTION;
  //     }
  //   }
  //
  // #undef READ_BYTE
  // #undef READ_CONSTANT
  // #undef BINARY_OP
}
