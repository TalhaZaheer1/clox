#ifndef CHUNK_H
#define CHUNK_H

#include "../common.h"
#include "value.h"

typedef enum { OP_RETURN, OP_CONSTANT } OpCode;

typedef struct {
  int count;
  int capacity;
  int *lines;
  uint8_t *code;
  ValueArray constantArr;
} Chunk;

void initChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t instruction, int line);
uint8_t addConstant(Chunk *chunk, int constant);
void freeChunk(Chunk *chunk);

#endif
