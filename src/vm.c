#include "common.h"
#include "vm.h"
#include "debug.h"
#include "compiler.h"
#include "object.h"
#include "memory.h"
#include "fileIO.h"
#include "macros.h"

#include <SDL2/SDL.h>

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

VM vm;

static void runtime_error(const char* format, ...);

static Value clock_native(int arg_count, Value* args) {
    unused(arg_count); unused(args);
    return NUMBER_VAL((double) clock() / CLOCKS_PER_SEC);
}

static Value rand_native(int arg_count, Value* args) {
    if (arg_count > 1)
        runtime_error("Too many arguments.");

    if (!IS_NUMBER(args[0]))
        runtime_error("First argument is not a number.");

    int max = AS_NUMBER(args[0]);

    int upper = max;
    int lower = 0;

    int num = (rand() % (upper - lower + 1)) + lower;
    return NUMBER_VAL(num);
}

static Value read_native(int arg_count, Value* args) {
    if (arg_count > 1)
        runtime_error("Too many arguments.");

    if (!IS_STRING(args[0]))
        runtime_error("First argument is not a string.");

    char* path = AS_CSTRING(args[0]);
    char* contents = read_file(path);

    ObjString* ret = take_string(contents, strlen(contents));

    return OBJ_VAL(ret);
}

#define BOARD_SIZE 80
static SDL_Window* window;
static SDL_Surface* window_surface;
static uint8_t board[BOARD_SIZE * BOARD_SIZE];
static uint8_t board2[BOARD_SIZE * BOARD_SIZE];

static Value sdl_init_native(int arg_count, Value* args) {
    
    if (arg_count > 3)
        runtime_error("Too many arguments.");

    if (!IS_NUMBER(args[0]))
        runtime_error("First argument is not a number.");

    if (!IS_NUMBER(args[1]))
        runtime_error("Second argument is not a number.");

    if (!IS_STRING(args[2]))
        runtime_error("Third argument is not a number.");

    int width = AS_NUMBER(args[0]);
    int height = AS_NUMBER(args[1]);

    char* title = AS_CSTRING(args[2]);

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        runtime_error("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    window = SDL_CreateWindow(title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width, height,
        0);

    if(!window) {
        runtime_error("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }

    window_surface = SDL_GetWindowSurface(window);

    if (!window_surface) {
        runtime_error("Window surface could not be created! SDL_Error: %s\n", SDL_GetError());
    }

    return NULL_VAL;
}

static Value sdl_clear_native(int arg_count, Value* args) {
    unused(arg_count); unused(args);

    SDL_FillRect(window_surface, NULL, SDL_MapRGB(window_surface->format, 0x00, 0x00, 0x00));

    return NULL_VAL;
}

static Value sdl_update_native(int arg_count, Value* args) {
    unused(arg_count); unused(args);

    SDL_UpdateWindowSurface(window);

    return NULL_VAL;
}

static Value sdl_is_not_quit_native(int arg_count, Value* args) {
    unused(arg_count); unused(args);

    SDL_Event event;
    SDL_PollEvent(&event);
    return BOOL_VAL(event.type != SDL_QUIT);
}

static Value sdl_quit_native(int arg_count, Value* args) {
    unused(arg_count); unused(args);

    SDL_Quit();
    return NULL_VAL;
}

static Value sdl_draw_rect_native(int arg_count, Value* args) {
    if (arg_count > 4)
        runtime_error("Too many arguments.");

    if (!IS_NUMBER(args[0]))
        runtime_error("First argument is not a number.");

    if (!IS_NUMBER(args[1]))
        runtime_error("Second argument is not a number.");

    if (!IS_NUMBER(args[2]))
        runtime_error("Third argument is not a number.");

    if (!IS_NUMBER(args[3]))
        runtime_error("Fourth argument is not a number.");

    int x = AS_NUMBER(args[0]);
    int y = AS_NUMBER(args[1]);
    int w = AS_NUMBER(args[2]);
    int h = AS_NUMBER(args[3]);

    SDL_Rect rect = {x, y, w, h};
    SDL_FillRect(window_surface, &rect, SDL_MapRGB(window_surface->format, 0xFF, 0xFF, 0xFF));

    return NULL_VAL;
}

static Value set_cell_native(int arg_count, Value* args) {
    if (arg_count > 3)
        runtime_error("Too many arguments.");

    if (!IS_NUMBER(args[0]))
        runtime_error("First argument is not a number.");

    if (!IS_NUMBER(args[1]))
        runtime_error("Second argument is not a number.");

    if (!IS_NUMBER(args[2]))
        runtime_error("Third argument is not a number.");

    int row = AS_NUMBER(args[0]);
    int col = AS_NUMBER(args[1]);
    int value = AS_NUMBER(args[2]);

    if (row < 0 || row >= BOARD_SIZE)
        runtime_error("First argument is out of bounds.");

    if (col < 0 || col >= BOARD_SIZE)
        runtime_error("Second argument is out of bounds.");

    board2[row * BOARD_SIZE + col] = value;

    return NULL_VAL;
}

static Value get_cell_native(int arg_count, Value* args) {
    if (arg_count > 2)
        runtime_error("Too many arguments.");

    if (!IS_NUMBER(args[0]))
        runtime_error("First argument is not a number.");

    if (!IS_NUMBER(args[1]))
        runtime_error("Second argument is not a number.");

    int row = AS_NUMBER(args[0]);
    int col = AS_NUMBER(args[1]);

    if (row < 0 || row >= BOARD_SIZE)
        runtime_error("First argument is out of bounds.");

    if (col < 0 || col >= BOARD_SIZE)
        runtime_error("Second argument is out of bounds.");

    int num = board[row * BOARD_SIZE + col];

    return NUMBER_VAL(num);
}

static Value push_board_native(int arg_count, Value* args) {
    unused(arg_count); unused(args);

    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        board[i] = board2[i];
    }
    
    for (int i = 0; i < BOARD_SIZE * BOARD_SIZE; i++) {
        board2[i] = 0;
    }

    return NULL_VAL;
}




/*
static Value eval_native(int arg_count, Value* args) {    
    if (arg_count > 1)
        runtime_error("Too many arguments.");

    if (!IS_STRING(args[0]))
        runtime_error("First argument is not a string.");

    InterpretResult ret = interpret(AS_CSTRING(args[0]));

    if (ret != INTERPRET_OK)
        runtime_error("Error while evaluating string.");

    return NUMBER_VAL((int) ret);
}
*/


static void reset_stack() {
    vm.stack_top = vm.stack;
    vm.frame_count = 0;
    vm.open_upvalues = NULL;
}

static Value peek(int distance) {
    return vm.stack_top[-1 - distance];
}

static bool call(ObjClosure* closure, int arg_count) {
    if (arg_count != closure->function->arity) {
        runtime_error("Expected %d arguments but got %d.", closure->function->arity, arg_count);
    }

    if (vm.frame_count == FRAMES_MAX) {
        runtime_error("Stack overflow.");
        return false;
    }

    CallFrame* frame = &vm.frames[vm.frame_count++];
    frame->closure = closure;
    frame->ip = closure->function->chunk.code;
    frame->slots = vm.stack_top - arg_count - 1;

    return true;
}

static bool call_value(Value callee, int arg_count) {
    if (IS_OBJ(callee)) {
        switch (OBJ_TYPE(callee)) {
        case OBJ_CLOSURE:
            return call(AS_CLOSURE(callee), arg_count);
        case OBJ_NATIVE: {
            NativeFn native = AS_NATIVE(callee);
            Value result = native(arg_count, vm.stack_top - arg_count);
            vm.stack_top -= arg_count + 1;
            push(result);
            return true;
        }
        default:
            break; // Non-callable object type.
        }
    }
    runtime_error("Can only call functions and classes.");
    return false;
}

static ObjUpvalue* capture_upvalue(Value* local) {
    ObjUpvalue* prev_upvalue = NULL;
    ObjUpvalue* upvalue = vm.open_upvalues;
    while (upvalue != NULL && upvalue->location > local) {
        prev_upvalue = upvalue;
        upvalue = upvalue->next;
    }

    if (upvalue != NULL && upvalue->location == local) {
        return upvalue;
    }

    ObjUpvalue* created_upvalue = new_upvalue(local);

    created_upvalue->next = upvalue;

    if (prev_upvalue == NULL) {
        vm.open_upvalues = created_upvalue;
    } else {
        prev_upvalue->next = created_upvalue;
    }

    return created_upvalue;
}

static void close_upvalues(Value* last) {
    while (vm.open_upvalues != NULL && vm.open_upvalues->location >= last) {
        ObjUpvalue* upvalue = vm.open_upvalues;
        upvalue->closed = *upvalue->location;
        upvalue->location = &upvalue->closed;
        vm.open_upvalues = upvalue->next;
    }
}

static bool is_falsey(Value value) {
  return IS_NULL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static void concatenate() {
    ObjString* b = AS_STRING(peek(0));
    ObjString* a = AS_STRING(peek(1));

    int length = a->length + b->length;
    char* chars = ALLOCATE(char, length + 1);
    memcpy(chars, a->chars, a->length);
    memcpy(chars + a->length, b->chars, b->length);
    chars[length] = '\0';

    ObjString* result = take_string(chars, length);
    pop();
    pop();
    push(OBJ_VAL(result));
}

static void runtime_error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    for (int i = vm.frame_count - 1; i >= 0; i--) {
        CallFrame* frame = &vm.frames[i];
        ObjFunction* function = frame->closure->function;
        size_t instruction = frame->ip - function->chunk.code - 1;
        fprintf(stderr, "[line %d] in ", function->chunk.lines[instruction]);
        if (function->name == NULL) {
            fprintf(stderr, "script\n");
        } else {
            fprintf(stderr, "%s()\n", function->name->chars);
        }
    }
    reset_stack();
}

static void define_native(const char* name, NativeFn function) {
    push(OBJ_VAL(copy_string(name, (int) strlen(name))));
    push(OBJ_VAL(new_native(function)));
    table_set(&vm.globals, AS_STRING(vm.stack[0]), vm.stack[1]);
    pop();
    pop();
}

static InterpretResult run() {
    CallFrame* frame = &vm.frames[vm.frame_count - 1];

    #define READ_BYTE() (*frame->ip++)

    #define READ_SHORT() (frame->ip += 2, (uint16_t) ((frame->ip[-2] << 8) | frame->ip[-1]))

    #define READ_CONSTANT() (frame->closure->function->chunk.constants.values[READ_BYTE()])

    #define READ_STRING() AS_STRING(READ_CONSTANT())

    #define BINARY_OP(value_type, op) \
        do { \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
            runtime_error("Operands must be numbers."); \
            return INTERPRET_RUNTIME_ERROR; \
        } \
        double b = AS_NUMBER(pop()); \
        double a = AS_NUMBER(pop()); \
        push(value_type(a op b)); \
        } while (false)

    for (;;) {

        #ifdef DEBUG_TRACE_EXECUTION
            printf("          ");
            for (Value* slot = vm.stack; slot < vm.stack_top; slot++) {
                printf("[ ");
                print_value(*slot);
                printf(" ]");
            }
            printf("\n");
            disassemble_instruction(&frame->closure->function->chunk, (int) (frame->ip - frame->closure->function->chunk.code));
        #endif

        uint8_t instruction;

        switch (instruction = READ_BYTE()) {
        case OP_CONSTANT: {
            Value constant = READ_CONSTANT();
            push(constant);
            #ifdef DEBUG_TRACE_EXECUTION
                print_value(constant);
                printf("\n");
            #endif
            break;
        }
        case OP_NULL:  push(NULL_VAL); break;
        case OP_TRUE:  push(BOOL_VAL(true)); break;
        case OP_FALSE: push(BOOL_VAL(false)); break;
        case OP_POP:   pop(); break;
        case OP_GET_LOCAL: {
            uint8_t slot = READ_BYTE();
            push(frame->slots[slot]);
            break;
        }
        case OP_SET_LOCAL: {
            uint8_t slot = READ_BYTE();
            frame->slots[slot] = peek(0);
            break;
        }
        case OP_GET_GLOBAL: {
            ObjString* name = READ_STRING();
            Value value;
            if (!table_get(&vm.globals, name, &value)) {
                runtime_error("Undefined variable '%s'.", name->chars);
                return INTERPRET_RUNTIME_ERROR;
            }
            push(value);
            break;
        }
        case OP_DEFINE_GLOBAL: {
            ObjString* name = READ_STRING();
            table_set(&vm.globals, name, peek(0));
            pop();
            break;
        }
        case OP_SET_GLOBAL: {
            ObjString* name = READ_STRING();
            if (table_set(&vm.globals, name, peek(0))) {
                table_delete(&vm.globals, name); 
                runtime_error("Undefined variable '%s'.", name->chars);
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        }
        case OP_GET_UPVALUE: {
            uint8_t slot = READ_BYTE();
            push(*frame->closure->upvalues[slot]->location);
            break;
        }
        case OP_EQUAL: {
            Value b = pop();
            Value a = pop();
            push(BOOL_VAL(values_equal(a, b)));
            break;
        }
        case OP_GREATER:  BINARY_OP(BOOL_VAL, >); break;
        case OP_LESS:     BINARY_OP(BOOL_VAL, <); break;
        case OP_ADD: {
            if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
                concatenate();
            } else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
                double b = AS_NUMBER(pop());
                double a = AS_NUMBER(pop());
                push(NUMBER_VAL(a + b));
            } else {
                runtime_error("Operands must be two numbers or two strings.");
                return INTERPRET_RUNTIME_ERROR;
            }
            break;
        }
        case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
        case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *); break;
        case OP_DIVIDE:   BINARY_OP(NUMBER_VAL, /); break;
        case OP_NOT:
            push(BOOL_VAL(is_falsey(pop())));
            break;
        case OP_NEGATE:
            if (!IS_NUMBER(peek(0))) {
                runtime_error("Operand must be a number.");
                return INTERPRET_RUNTIME_ERROR;
            }
            push(NUMBER_VAL(-AS_NUMBER(pop())));
            break;
        case OP_POSITIVE:
            if (!IS_NUMBER(peek(0))) {
                runtime_error("Operand must be a number.");
                return INTERPRET_RUNTIME_ERROR;
            }
            push(NUMBER_VAL(AS_NUMBER(pop())));
            break;
        case OP_PRINT: 
            print_value(pop());
            printf("\n");
            break;
        case OP_JUMP: {
            uint16_t offset = READ_SHORT();
            frame->ip += offset;
            break;
        }
        case OP_JUMP_IF_FALSE: {
            uint16_t offset = READ_SHORT();
            if (is_falsey(peek(0))) frame->ip += offset;
            break;
        }
        case OP_LOOP: {
            uint16_t offset = READ_SHORT();
            frame->ip -= offset;
            break;
        }
        case OP_CALL: {
            int arg_count = READ_BYTE();
            if (!call_value(peek(arg_count), arg_count)) {
                return INTERPRET_RUNTIME_ERROR;
            }
            frame = &vm.frames[vm.frame_count - 1];
            break;
        }
        case OP_CLOSURE: {
            ObjFunction* function = AS_FUNCTION(READ_CONSTANT());
            ObjClosure* closure = new_closure(function);
            push(OBJ_VAL(closure));
            for (int i = 0; i < closure->upvalue_count; i++) {
                uint8_t is_local = READ_BYTE();
                uint8_t index = READ_BYTE();
                if (is_local) {
                    closure->upvalues[i] = capture_upvalue(frame->slots + index);
                } else {
                    closure->upvalues[i] = frame->closure->upvalues[index];
                }
            }
            break;
        }
        case OP_CLOSE_UPVALUE:
            close_upvalues(vm.stack_top - 1);
            pop();
            break;
        case OP_RETURN: {
            Value result = pop();
            close_upvalues(frame->slots);
            vm.frame_count--;
            if (vm.frame_count == 0) {
                pop();
                return INTERPRET_OK;
            }

            vm.stack_top = frame->slots;
            push(result);
            frame = &vm.frames[vm.frame_count - 1];
            break;
        }
        default:
            runtime_error("Unknown opcode %d", instruction);
            return INTERPRET_RUNTIME_ERROR;
        }
    }

    #undef READ_BYTE
    #undef READ_CONSTANT
    #undef READ_SHORT
    #undef READ_STRING
    #undef BINARY_OP
}

void init_vm() {
    reset_stack();
    vm.objects = NULL;
    vm.bytes_allocated = 0;
    vm.next_gc = 1024 * 1024;

    vm.gray_count = 0;
    vm.gray_capacity = 0;
    vm.gray_stack = NULL;

    init_table(&vm.globals);
    init_table(&vm.strings);

    srand(time(NULL));

    define_native("clock", clock_native);
    //define_native("sleep", sleep_native);
    define_native("random", rand_native);
    define_native("read", read_native);
    define_native("sdl_init", sdl_init_native);
    define_native("sdl_update", sdl_update_native);
    define_native("sdl_is_not_quit", sdl_is_not_quit_native);
    define_native("sdl_quit", sdl_quit_native);
    define_native("sdl_draw_rect", sdl_draw_rect_native);
    define_native("sdl_clear", sdl_clear_native);

    define_native("set_cell", set_cell_native);
    define_native("get_cell", get_cell_native);
    define_native("push_board", push_board_native);
    //define_native("init_board", init_board_native);
}

void free_vm() {
    free_table(&vm.globals);
    free_table(&vm.strings);
    free_objects();
}

void push(Value value) {
    *vm.stack_top = value;
    vm.stack_top++;
}

Value pop() {
    vm.stack_top--;
    return *vm.stack_top;
}

InterpretResult interpret(const char* source) {
    ObjFunction* function = compile(source);
    if (function == NULL) return INTERPRET_COMPILE_ERROR;

    push(OBJ_VAL(function));
    ObjClosure* closure = new_closure(function);
    pop();
    push(OBJ_VAL(closure));
    call(closure, 0);

    return run();
}
