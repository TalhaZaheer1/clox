#include "./compiler.h"
#include "scanner.h"

void expression();

typedef enum {
  PREC_NONE,
  PREC_ASSIGNMENT, // =
  PREC_OR,         // or
  PREC_AND,        // and
  PREC_EQUALITY,   // == !=
  PREC_COMPARISON, // < > <= >=
  PREC_TERM,       // + -
  PREC_FACTOR,     // * /
  PREC_UNARY,      // ! -
  PREC_CALL,       // . ()
  PREC_PRIMARY
} Precedence;

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

static void binary() {
  TokenType type = parser.previous.type;

  expression();

  switch (type) {
  case TOKEN_MINUS:
    emitByte(OP_SUBSTRACT);
    break;
  case TOKEN_PLUS:
    emitByte(OP_ADD);
    break;
  case TOKEN_STAR:
    emitByte(OP_MULTIPLY);
    break;
  case TOKEN_SLASH:
    emitByte(OP_DIVIDE);
    break;
  default:
    return;
  }
}

static void unary() {
  TokenType type = parser.previous.type;

  expression();

  switch (type) {
  case TOKEN_MINUS:
    emitByte(OP_NEGATE);
    break;
  default:
    return;
  }
}

void parsePrecedence() {}

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
