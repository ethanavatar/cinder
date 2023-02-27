#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "scanner.h"

#include "macros.h"

bool compile(const char* source, Chunk* chunk) {
    unused(chunk);

    init_scanner(source);
    //advance();
    //expression();
    //consume(TOKEN_EOF, "Expect end of expression.");

    // temporary return value
    return true;
}
