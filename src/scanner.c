#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

struct Scanner {
  const char* start;
  const char* current;
  int line;
};

static struct Scanner scanner;

static char advance() {
    scanner.current++;
    return scanner.current[-1];
}

static bool is_at_end() {
    return *scanner.current == '\0';
}

static struct Token make_token(enum TokenType type) {
    struct Token token;
    token.type = type;
    token.start = scanner.start;
    token.length = (int) (scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}

static struct Token error_token(const char* message) {
    struct Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int) strlen(message);
    token.line = scanner.line;
    return token;
}

void init_scanner(const char* source) {
    scanner.start = source;
    scanner.current = source;
    scanner.line = 1;
}

struct Token scanToken() {
    scanner.start = scanner.current;

    if (is_at_end()) return make_token(TOKEN_EOF);

    char c = advance();
    switch (c) {
    default:
        break;
    }

    return error_token("Unexpected character.");
}

