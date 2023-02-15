#include <stdlib.h>

#include "chunk.h"
#include "memory.h"
#include "value.h"

void init_chunk(struct Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    init_value_array(&chunk->constants);
    chunk->lines = NULL;
}


void write_chunk(struct Chunk* chunk, uint8_t byte, int line) {
    // if the chunk is full, grow the chunk
    if (chunk->capacity < chunk->count + 1) {

        // save the old capacity
        int oldCapacity = chunk->capacity;

        // grow the chunk
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
    }

    // write the byte to the chunk and increment the index
    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

int add_constant(struct Chunk* chunk, Value value) {
  write_value_array(&chunk->constants, value);
  return chunk->constants.count - 1;
}


void free_chunk(struct Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    init_chunk(chunk);
    free_value_array(&chunk->constants);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
}

