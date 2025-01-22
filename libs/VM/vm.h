#ifndef VM_H
#define VM_H

#include "../common.h"
#include "../representation/chunk.h"
#define MAX_STACK 256

typedef struct {
  Chunk *chunk;
  Value stack[MAX_STACK];
  Value *stackTop;
  uint8_t *ip;
  Obj *objects;
} VM;

typedef enum {
  INTERPRET_OK = 10,
  INTERPRET_RUNTIME_EXCEPTION,
  INTERPRET_COMPILETIME_EXCEPTION
} InterpretResult;

extern VM vm;

void initVM();
void freeVM();
InterpretResult interpret(char *source);
void push(Value value);
Value pop();
void resetStack();
void freeObjects();

#endif
