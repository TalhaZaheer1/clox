#include "./compiler.h"
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  Token previous;
  Token current;
  bool hadError;
  bool panicMode;
} Parser;

Parser parser;
Chunk *compilingChunk;

static Chunk *currentChunk() { return compilingChunk; }

static void errorAt(Token *token, char *message) {
  if (parser.panicMode)
    return;
  parser.panicMode = true;

  fprintf(stderr, "[Line %d] Error", token->line);
  if (token->type == TOKEN_EOF) {
    fprintf(stderr, " at end");
  } else if (token->type == TOKEN_ERROR) {

  } else {
    fprintf(stderr, " at '%.*s'", token->length, token->lexeme);
  }

  fprintf(stderr, ": %s\n", message);
  parser.hadError = true;
}

static void error(char *message) { errorAt(&parser.previous, message); }

static void errorAtCurrent(char *message) { errorAt(&parser.current, message); }

static void emitByte(OpCode code) {
  writeChunk(currentChunk(), code, parser.previous.line);
}

static void emitBytes(OpCode code1, OpCode code2) {
  emitByte(code1);
  emitByte(code2);
}

static void emitReturn() { emitByte(OP_RETURN); }
static void endCompiler() { emitReturn(); }

static void expression() {}

static uint8_t makeConstant(double num) {
  int indx = addConstant(currentChunk(), num);
  if (indx > UINT8_MAX) {
    error("Too many constants in this chunk.");
    return 0;
  }
  return (uint8_t)indx;
}

static void emitConstant(Value value) {
  emitBytes(OP_CONSTANT, makeConstant(value));
}

static void number() {
  double num = strtod(parser.previous.lexeme, NULL);
  emitConstant(num);
}

static void grouping() {
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expected ')' after expression.");
}

static void advance() {
  parser.previous = parser.current;

  for (;;) {
    parser.current = scanToken();

    if (parser.current.type != TOKEN_ERROR)
      break;

    errorAtCurrent("Compile time exception.");
  }
}

static void consume(TokenType type, char *message) {
  if (parser.current.type == type) {
    advance();
  }

  errorAtCurrent(message);
}

bool compile(char *source, Chunk *chunk) {
  initScanner(source);
  compilingChunk = chunk;
  parser.hadError = false;
  parser.panicMode = false;

  advance();

  consume(TOKEN_EOF, "Expected end of expression.");
  return !parser.hadError;
}
