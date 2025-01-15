#ifndef DISASSEMBLE_H
#define DISASSEMBLE_H

#include "../common.h"
#include "../representation/chunk.h"

void disassembleChunk(Chunk *chunk, char *title);
int disassembleInstruction(Chunk *chunk, int offset);

// helpers
int simpleInstruction(char *instrc, int offset);
int oneOperandInstruction(char *instrc, Chunk *chunk, int offset);
int binaryInstruction(char *instrc, Chunk *chunk, int offset);
#endif // !DISASSEMBLE_H
