#include "./disassembler.h"

void disassembleChunk(Chunk *chunk, char *title) {
  printf("== %s ==\n", title);
  for (int offset = 0; offset < chunk->count;) {
    offset = disassembleInstruction(chunk, offset);
  }
}

int disassembleInstruction(Chunk *chunk, int offset) {
  printf("%04d ", chunk->code[offset]);
  if (offset > 0 && (chunk->lines[offset] == chunk->lines[offset - 1]))
    printf("   | ");
  else
    printf("%4d ", chunk->lines[offset]);

  switch (chunk->code[offset]) {
  case OP_RETURN:
    return simpleInstruction("OP_RETURN", offset);
  case OP_CONSTANT:
    return oneOperandInstruction("OP_CONSTANT", chunk, offset);
  case OP_ADD:
    return simpleInstruction("OP_ADD", offset);
  case OP_SUBSTRACT:
    return simpleInstruction("OP_SUBSTRACT", offset);
  case OP_MULTIPLY:
    return simpleInstruction("OP_MULTIPLY", offset);
  case OP_DIVIDE:
    return simpleInstruction("OP_DIVIDE", offset);
  case OP_NEGATE:
    return simpleInstruction("OP_NEGATE", offset);
  default:
    printf("Invalid Instruction: %d\n", chunk->code[offset]);
    return offset + 1;
  }
}

int simpleInstruction(char *instrc, int offset) {
  printf("%s\n", instrc);
  return offset + 1;
}

int oneOperandInstruction(char *instrc, Chunk *chunk, int offset) {
  uint8_t operand = chunk->code[offset + 1];
  printf("%-16s %4d '", instrc, operand);
  printValue(chunk->constantArr.values[operand]);
  printf("'\n");
  return offset + 2;
}
