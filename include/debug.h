#ifndef cinder_debug_h
#define cinder_debug_h

#include "chunk.h"

// prints the given chunk
void disassemble_chunk(struct Chunk* chunk, const char* name);



// prints the instruction at the given offset within the given chunk
int disassemble_instruction(struct Chunk* chunk, int offset);

#endif
