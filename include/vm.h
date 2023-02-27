#ifndef cinder_vm_h
#define cinder_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    struct Chunk* chunk;
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value* stackTop;
} VM;

enum InterpretResult {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
};

void init_vm();
void free_vm();

enum InterpretResult interpret(Chunk* chunk);

void push(Value value);
Value pop();

#endif
