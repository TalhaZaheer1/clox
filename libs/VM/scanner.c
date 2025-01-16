#include "scanner.h"
#include <string.h>

Scanner scanner;
static char advance() {
  scanner.current++;
  return scanner.current[-1];
}

void initScanner(char *source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}
static Token number() {
  while (isDigit(peek()))
    advance();
  if (peek() == '.' && isDigit(peekNext())) {
    advance();
    while (isDigit(peek()))
      advance();
  }
  return makeToken(TOKEN_NUMBER);
}

Token scanToken() {
  ignoreWhiteSpaces();
  scanner.start = scanner.current;

  if (isAtEnd())
    return makeToken(TOKEN_EOF);

  char c = advance();

  if (isDigit(c))
    return number();
  if (isAlpha(c))
    return identifier();

  switch (c) {
  case '(':
    return makeToken(TOKEN_LEFT_PAREN);
  case ')':
    return makeToken(TOKEN_RIGHT_PAREN);
  case '{':
    return makeToken(TOKEN_LEFT_BRACE);
  case '}':
    return makeToken(TOKEN_RIGHT_BRACE);
  case ',':
    return makeToken(TOKEN_COMMA);
  case '.':
    return makeToken(TOKEN_DOT);
  case '-':
    return makeToken(TOKEN_MINUS);
  case '+':
    return makeToken(TOKEN_PLUS);
  case ';':
    return makeToken(TOKEN_SEMICOLON);
  case '*':
    return makeToken(TOKEN_STAR);
  case '!':
    return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
  case '=':
    return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
  case '>':
    return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
  case '<':
    return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
  case '/':
    if (match('/'))
      comment();
    else
      return errorToken("Expected '/' after character.");
    break;
  case '"':
    return string();
  }
  return errorToken("Unexpected character.");
}

bool isAtEnd() { return *scanner.current == '\0'; }

char peek() { return *scanner.current; }

bool match(char character) { return peek() == character; }

void comment() {
  while (peek() != '\n') {
    advance();
  }
}

void ignoreWhiteSpaces() {
  for (;;) {
    char c = peek();
    switch (c) {
    case ' ':
    case '\r':
    case '\t':
      advance();
      break;
    case '\n':
      scanner.line++;
      advance();
      break;
    default:
      return;
    }
  }
}

Token string() {
  while (!isAtEnd() && peek() != '"') {
    advance();
  }
  if (isAtEnd())
    return errorToken("Unterminated string.");
  advance();
  return makeToken(TOKEN_STRING);
}

Token makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.lexeme = scanner.start;
  token.length = scanner.current - scanner.start;
  token.line = scanner.line;
  return token;
}

Token errorToken(const char *message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.lexeme = message;
  token.length = strlen(message);
  token.line = scanner.line;
  return token;
}

bool isDigit(char c) { return c > '0' && c < '9'; }
bool isAlpha(char c) {
  return (c > 'a' && c < 'z') || (c > 'A' && c < 'Z') || c == '_';
}

char peekNext() {
  if (isAtEnd())
    return '\0';
  return scanner.current[1];
}

Token identifier() {
  while (isAlpha(peek()) || isDigit(peek())) {
    advance();
  }
  return makeToken(identifierType());
}

TokenType identifierType() {
  switch (scanner.start[0]) {
  case 'a':
    return checkKeyword(1, 2, "nd", TOKEN_AND);
  case 'c':
    return checkKeyword(1, 4, "lass", TOKEN_CLASS);
  case 'e':
    return checkKeyword(1, 3, "lse", TOKEN_ELSE);
  case 'i':
    return checkKeyword(1, 1, "f", TOKEN_IF);
  case 'n':
    return checkKeyword(1, 2, "il", TOKEN_NIL);
  case 'o':
    return checkKeyword(1, 1, "r", TOKEN_OR);
  case 'p':
    return checkKeyword(1, 4, "rint", TOKEN_PRINT);
  case 'r':
    return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
  case 's':
    return checkKeyword(1, 4, "uper", TOKEN_SUPER);
  case 'v':
    return checkKeyword(1, 2, "ar", TOKEN_VAR);
  case 'w':
    return checkKeyword(1, 4, "hile", TOKEN_WHILE);
  case 'f':
    switch (scanner.start[1]) {
    case 'a':
      return checkKeyword(2, 3, "lse", TOKEN_FALSE);
    case 'o':
      return checkKeyword(2, 1, "r", TOKEN_RETURN);
    case 'u':
      return checkKeyword(2, 1, "n", TOKEN_FUN);
    }
    break;
  case 't':
    switch (scanner.start[1]) {
    case 'h':
      return checkKeyword(2, 2, "is", TOKEN_THIS);
    case 'r':
      return checkKeyword(2, 2, "ue", TOKEN_TRUE);
    }
    break;
  }

  return TOKEN_IDENTIFIER;
}

TokenType checkKeyword(int start, int length, char *str, TokenType type) {
  if (scanner.current - scanner.start == start + length &&
      memcmp(scanner.start + start, str, length) == 0) {
    return type;
  }
  return TOKEN_IDENTIFIER;
}
