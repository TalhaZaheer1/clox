#include "./compiler.h"
#include "scanner.h"
#include <stdio.h>
#include <string.h>

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

typedef void (*ParseFn)();

typedef struct {
  ParseFn prefix;
  ParseFn infix;
  Precedence precedence;
} ParseRule;

Parser parser;
Chunk *compilingChunk;
void expression();

void parsePrecedence(Precedence precedence);

void binary();
void number();
void unary();
void grouping();

ParseRule rules[] = {
    [TOKEN_LEFT_PAREN] = {&grouping, NULL, PREC_NONE},
    [TOKEN_RIGHT_PAREN] = {NULL, NULL, PREC_NONE},
    [TOKEN_LEFT_BRACE] = {NULL, NULL, PREC_NONE},
    [TOKEN_RIGHT_BRACE] = {NULL, NULL, PREC_NONE},
    [TOKEN_COMMA] = {NULL, NULL, PREC_NONE},
    [TOKEN_DOT] = {NULL, NULL, PREC_NONE},
    [TOKEN_MINUS] = {&unary, &grouping, PREC_TERM},
    [TOKEN_PLUS] = {NULL, binary, PREC_TERM},
    [TOKEN_SEMICOLON] = {NULL, NULL, PREC_NONE},
    [TOKEN_SLASH] = {NULL, binary, PREC_FACTOR},
    [TOKEN_STAR] = {NULL, binary, PREC_FACTOR},
    [TOKEN_BANG] = {NULL, NULL, PREC_NONE},
    [TOKEN_BANG_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_EQUAL_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_GREATER] = {NULL, NULL, PREC_NONE},
    [TOKEN_GREATER_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_LESS] = {NULL, NULL, PREC_NONE},
    [TOKEN_LESS_EQUAL] = {NULL, NULL, PREC_NONE},
    [TOKEN_IDENTIFIER] = {NULL, NULL, PREC_NONE},
    [TOKEN_STRING] = {NULL, NULL, PREC_NONE},
    [TOKEN_NUMBER] = {number, NULL, PREC_NONE},
    [TOKEN_AND] = {NULL, NULL, PREC_NONE},
    [TOKEN_CLASS] = {NULL, NULL, PREC_NONE},
    [TOKEN_ELSE] = {NULL, NULL, PREC_NONE},
    [TOKEN_FALSE] = {NULL, NULL, PREC_NONE},
    [TOKEN_FOR] = {NULL, NULL, PREC_NONE},
    [TOKEN_FUN] = {NULL, NULL, PREC_NONE},
    [TOKEN_IF] = {NULL, NULL, PREC_NONE},
    [TOKEN_NIL] = {NULL, NULL, PREC_NONE},
    [TOKEN_OR] = {NULL, NULL, PREC_NONE},
    [TOKEN_PRINT] = {NULL, NULL, PREC_NONE},
    [TOKEN_RETURN] = {NULL, NULL, PREC_NONE},
    [TOKEN_SUPER] = {NULL, NULL, PREC_NONE},
    [TOKEN_THIS] = {NULL, NULL, PREC_NONE},
    [TOKEN_TRUE] = {NULL, NULL, PREC_NONE},
    [TOKEN_VAR] = {NULL, NULL, PREC_NONE},
    [TOKEN_WHILE] = {NULL, NULL, PREC_NONE},
    [TOKEN_ERROR] = {NULL, NULL, PREC_NONE},
    [TOKEN_EOF] = {NULL, NULL, PREC_NONE},
};

ParseRule *getRule(TokenType type) { return &rules[type]; }

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

static void advance() {
  parser.previous = parser.current;

  for (;;) {
    parser.current = scanToken();

    if (parser.current.type != TOKEN_ERROR)
      break;

    errorAtCurrent("Compile time exception.");
  }
}

static void emitByte(OpCode code) {
  writeChunk(currentChunk(), code, parser.previous.line);
}

static void emitBytes(OpCode code1, OpCode code2) {
  emitByte(code1);
  emitByte(code2);
}

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

static void emitReturn() { emitByte(OP_RETURN); }
static void endCompiler() { emitReturn(); }

void binary() {
  TokenType type = parser.previous.type;

  parsePrecedence(getRule(type)->precedence + 1);

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

void unary() {
  TokenType type = parser.previous.type;

  parsePrecedence(PREC_UNARY);

  switch (type) {
  case TOKEN_MINUS:
    emitByte(OP_NEGATE);
    break;
  default:
    return;
  }
}
void number() {
  double num = strtod(parser.previous.lexeme, NULL);
  emitConstant(num);
}

void grouping() {
  expression();
  consume(TOKEN_RIGHT_PAREN, "Expected ')' after expression.");
}

void parsePrecedence(Precedence precedence) {
  advance();
  ParseFn prefix;

  if ((prefix = getRule(parser.previous.type)->prefix) == NULL) {
    error("Expected expression.");
    return;
  }

  prefix();

  while (precedence <= getRule(parser.current.type)->precedence) {
    advance();
    ParseFn infix = getRule(parser.previous.type)->infix;
    infix();
  }
}

static void consume(TokenType type, char *message) {
  if (parser.current.type == type) {
    advance();
  } else
    errorAtCurrent(message);
}

void expression() { parsePrecedence(PREC_ASSIGNMENT); }

bool compile(char *source, Chunk *chunk) {
  initScanner(source);
  compilingChunk = chunk;
  parser.hadError = false;
  parser.panicMode = false;

  advance();
  expression();
  emitReturn();

#ifdef DEBUG_PRINT_CODE

#endif /* ifdef DEBUG_PRINT_CODE                                               \
        */

  consume(TOKEN_EOF, "Expected end of expression.");
  return !parser.hadError;
}
