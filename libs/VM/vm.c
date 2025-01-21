#include "./vm.h"

#include "../disassembler/disassembler.h"
#include "./compiler.h"
#include <stdarg.h>

#define DEBUG_TRACE_EXECUTION

VM vm;

void initVM() { resetStack(); }

void freeVM() {}

void resetStack() { vm.stackTop = vm.stack; }

static void runtimeError(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  fputs("\n", stderr);

  size_t instruction = vm.ip - vm.chunk->code - 1;
  int line = vm.chunk->lines[instruction];
  fprintf(stderr, "[line %d] in script\n", line);
  resetStack();
}
static Value peek(int distance) { return vm.stackTop[-1 - distance]; }

static bool isFalsey(Value val) {
  return IS_NIL(val) || (IS_BOOL(val) && !AS_BOOL(val));
}

static bool valuesEqual(Value a, Value b) {
  if (a.tag != b.tag)
    return false;
  switch (a.tag) {
  case VAL_BOOL:
    return AS_BOOL(a) == AS_BOOL(b);
  case VAL_NIL:
    return true;
  case VAL_NUMBER:
    return AS_NUMBER(a) == AS_NUMBER(b);
  default:
    return false;
  }
}

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

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constantArr.values[READ_BYTE()])
#define BINARY_OP(createType, op)                                              \
  do {                                                                         \
    Value valB = pop();                                                        \
    Value valA = pop();                                                        \
    if (IS_NUMBER(valA) && IS_NUMBER(valB)) {                                  \
      double a = AS_NUMBER(valA);                                              \
      double b = AS_NUMBER(valB);                                              \
      push(createType(a op b));                                                \
    } else {                                                                   \
      runtimeError("Unexpected type in binary operation.");                    \
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
    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));

#endif

    switch (instruction = READ_BYTE()) {

    case OP_NEGATE:
      if (IS_NUMBER(peek(0))) {
        double negatedNumber = -AS_NUMBER(peek(0));
        Value negatedValue = CREATE_NUMBER(negatedNumber);
        push(negatedValue);
      } else
        runtimeError("Unexcped type in negation.");
      break;
    case OP_NOT:
      push(CREATE_BOOLEAN(isFalsey(pop())));
      break;
    case OP_MULTIPLY:
      BINARY_OP(CREATE_NUMBER, *);
      break;
    case OP_DIVIDE:
      BINARY_OP(CREATE_NUMBER, /);
      break;
    case OP_ADD:
      BINARY_OP(CREATE_NUMBER, +);
      break;
    case OP_SUBSTRACT:
      BINARY_OP(CREATE_NUMBER, -);
      break;
    case OP_RETURN:
      printValue(pop());
      printf("\n");
      return INTERPRET_OK;
    case OP_CONSTANT:
      push(READ_CONSTANT());
      break;
    case OP_TRUE:
      push(CREATE_BOOLEAN(true));
      break;
    case OP_FALSE:
      push(CREATE_BOOLEAN(false));
      break;
    case OP_NIL:
      push(CREATE_NIL());
      break;
    case OP_EQUAL:
      push(CREATE_BOOLEAN(valuesEqual(pop(), pop())));
      break;
    case OP_GREATER:
      BINARY_OP(CREATE_BOOLEAN, >);
      break;
    case OP_LESS:
      BINARY_OP(CREATE_BOOLEAN, <);
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

  return run();
}
