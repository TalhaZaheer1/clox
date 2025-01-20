#include "chunk.h"
#include "../memory.h"

void initChunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  initValueArray(&chunk->constantArr);
}

void writeChunk(Chunk *chunk, uint8_t instruction, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code =
        GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
  }

  chunk->code[chunk->count] = instruction;
  chunk->lines[chunk->count++] = line;
}

int addConstant(Chunk *chunk, Value constant) {
  return writeValueArray(&chunk->constantArr, constant);
}

void freeChunk(Chunk *chunk) {
  chunk->lines = FREE_ARRAY(int, chunk->lines, chunk->capacity);
  chunk->code = FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  chunk->capacity = 0;
  chunk->count = 0;
  freeValueArray(&chunk->constantArr);
}
