#include "chunk.h"
#include "../memory.h"
#include <stdint.h>

void initChunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
}

void writeChunk(Chunk *chunk, uint8_t instruction) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code =
        GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
  }

  chunk->code[chunk->count++] = instruction;
}

void freeChunk(Chunk *chunk) {
  chunk->code = FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  chunk->capacity = 0;
  chunk->count = 0;
}
