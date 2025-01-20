#include "./vm.h"

#include "../disassembler/disassembler.h"
#include "./compiler.h"

#define DEBUG_TRACE_EXECUTION

VM vm;

void initVM() { resetStack(); }

void freeVM() {}

void resetStack() { vm.stackTop = vm.stack; }

static void runtimeException(char *message);

static Value peek(int distance) { return vm.stackTop[-1 - distance]; }

void push(Value value) {
  if (vm.stackTop - vm.stack > MAX_STACK)
    return;
  *vm.stackTop = value;
  vm.stackTop++;
}

Value pop() {
  if (vm.stackTop == vm.stack)
    return (Value){VAL_NIL, {.number = 0}};
  return *--vm.stackTop;
}

InterpretResult interpret(char *source) {
  Chunk chunk;
  initChunk(&chunk);
  initVM();

  if (!compile(source, &chunk)) {
    freeChunk(&chunk);
    return INTERPRET_COMPILETIME_EXCEPTION;
  }

  vm.chunk = &chunk;
  vm.ip = chunk.code;

#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (chunk.constantArr.values[READ_BYTE()])
#define BINARY_OP(op)                                                          \
  do {                                                                         \
    Value valB = pop();                                                        \
    Value valA = pop();                                                        \
    if (IS_NUMBER(valA) && IS_NUMBER(valB)) {                                  \
      double a = AS_NUMBER(valA);                                              \
      double b = AS_NUMBER(valB);                                              \
      push(CREATE_NUMBER(a + b));                                              \
    } else {                                                                   \
      runtimeException("Unexpected type in binary operation.");                \
    }                                                                          \
  } while (false)

  uint8_t instruction;
  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("           ");
    for (Value *itr = vm.stack; itr < vm.stackTop; itr++) {
      printf("[ ");
      printValue(*itr);
      printf(" ]");
    }
    printf("\n");
    disassembleInstruction(&chunk, (int)(vm.ip - chunk.code));

#endif

    switch (instruction = READ_BYTE()) {

    case OP_NEGATE:
      if (IS_NUMBER(peek(0))) {
        double negatedNumber = -AS_NUMBER(peek(0));
        Value negatedValue = CREATE_NUMBER(negatedNumber);
        push(negatedValue);
      } else
        runtimeException("Unexcped type in negation.");
      break;
    case OP_MULTIPLY:
      BINARY_OP(*);
      break;
    case OP_DIVIDE:
      BINARY_OP(/);
      break;
    case OP_ADD:
      BINARY_OP(+);
      break;
    case OP_SUBSTRACT:
      BINARY_OP(-);
      break;
    case OP_RETURN:
      printValue(pop());
      printf("\n");
      return INTERPRET_OK;
    case OP_CONSTANT:
      push(READ_CONSTANT());
      break;
    default:
      printf("Invalid Instruction.");
      return INTERPRET_RUNTIME_EXCEPTION;
    }
  }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}
