#ifndef CHUNK_H
#define CHUNK_H

#include "../common.h"

typedef enum { OP_RETURN } OpCode;

typedef struct {
  int count;
  int capacity;
  uint8_t *code;
} Chunk;

void initChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t instruction);
void freeChunk(Chunk *chunk);

#endif
