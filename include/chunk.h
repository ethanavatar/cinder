#ifndef cinder_chunk_h
#define cinder_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
    OP_CONSTANT,
    OP_NULL,
    OP_TRUE,
    OP_FALSE,
    OP_POP,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
    OP_GET_GLOBAL,
    OP_DEFINE_GLOBAL,
    OP_SET_GLOBAL,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_NEGATE,
    OP_PRINT,
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NOT,
    OP_LOOP,
    OP_CALL,
    OP_RETURN,
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    ValueArray constants;
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
