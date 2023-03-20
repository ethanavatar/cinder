#ifndef cinder_value_h
#define cinder_value_h

#include "common.h"

// an enum of the possible types of values in the interpreter.
typedef enum {
  VAL_BOOL,
  VAL_NIL, 
  VAL_NUMBER,
} ValueType;

// a value is a tagged union of the possible types of values in the interpreter.
typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
    } as;
} Value;

#define IS_BOOL(value)    ((value).type == VAL_BOOL)
#define IS_NIL(value)     ((value).type == VAL_NIL)
#define IS_NUMBER(value)  ((value).type == VAL_NUMBER)

#define AS_BOOL(value)    ((value).as.boolean)
#define AS_NUMBER(value)  ((value).as.number)

#define BOOL_VAL(value)   ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL           ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})

// a value array is a dynamically sized array of interpreter values.
typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

// compare two values for equality.
bool values_equal(Value a, Value b);

// initialize the given value array.
void init_value_array(ValueArray* array);

// write a value to the given value array.
void write_value_array(ValueArray* array, Value value);

// free the memory from the given value array.
void free_value_array(ValueArray* array);

// print the given value.
void print_value(Value value);

#endif
