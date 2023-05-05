#include <stdlib.h>

#include "chunk.h"
#include "memory.h"
#include "value.h"
#include "vm.h"

void init_chunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    init_value_array(&chunk->constants);
    chunk->lines = NULL;
}

void write_chunk(Chunk* chunk, uint8_t byte, int line) {
    // if the chunk is full, grow the chunk
    if (chunk->capacity < chunk->count + 1) {

        // save the old capacity
        int old_capacity = chunk->capacity;

        // grow the chunk
        chunk->capacity = GROW_CAPACITY(old_capacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, old_capacity, chunk->capacity);
    }

    // write the byte to the chunk and increment the index
    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

int add_constant(Chunk* chunk, Value value) {
    push(value);
    write_value_array(&chunk->constants, value);
    pop();
    return chunk->constants.count - 1;
}


void free_chunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    init_chunk(chunk);
    free_value_array(&chunk->constants);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
}

