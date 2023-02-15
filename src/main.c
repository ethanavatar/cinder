#include "common.h"
#include <stdio.h>
#include "scanner.h"
#include "macros.h"
#include "chunk.h"
#include "debug.h"

int32_t main(const int32_t argc, const char* argv[]) {
    unused(argc);
    unused(argv);

    struct Chunk chunk;
    init_chunk(&chunk);

    int constant = add_constant(&chunk, 1.2);
    write_chunk(&chunk, OP_CONSTANT, 123);
    write_chunk(&chunk, constant, 123);

    write_chunk(&chunk, OP_RETURN, 123);

    disassemble_chunk(&chunk, "test chunk");
    free_chunk(&chunk);
    return 0;
}
