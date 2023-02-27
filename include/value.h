#ifndef cinder_value_h
#define cinder_value_h

#include "common.h"

// the interpreter value type (double).
typedef double Value;

// a value array is a dynamically sized array of interpreter values.
typedef struct {
  int capacity;
  int count;
  Value* values;
} ValueArray;

// initialize the given value array.
void init_value_array(ValueArray* array);

// write a value to the given value array.
void write_value_array(ValueArray* array, Value value);

// free the memory from the given value array.
void free_value_array(ValueArray* array);

// print the given value.
void print_value(Value value);

#endif
