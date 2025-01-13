#ifndef VM_H
#define VM_H

#include "../common.h"
#include "../representation/chunk.h"

typedef struct {
  Chunk *chunk;
  uint8_t *ip;
} VM;

typedef enum {
  RETURN_OK,
  RETURN_RUNTIME_EXCEPTION,
  RETURN_COMPILETIME_EXCEPTION
} InterpretResult;

void initVM();
InterpretResult interpret(Chunk *chunk);
void freeVM();

#endif
