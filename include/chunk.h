#ifndef cinder_chunk_h
#define cinder_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_NEGATE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_RETURN,
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    struct ValueArray constants;
    int* lines;
} Chunk;

// initializes the given chunk
void init_chunk(Chunk* chunk);

// frees the given chunk
void free_chunk(Chunk* chunk);

// writes a byte to the given chunk
// if the chunk is full, grow the chunk
void write_chunk(Chunk* chunk, uint8_t byte, int line);

// adds a constant to the given chunk
int add_constant(Chunk* chunk, Value value);

#endif
