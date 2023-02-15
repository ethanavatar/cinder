#ifndef cinder_memory_h
#define cinder_memory_h

#include "common.h"

// grow the capacity of the chunks array.
#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity) * 2)

// grow the size of the chunks array.
#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*) reallocate(pointer, sizeof(type) * (oldCount), \
        sizeof(type) * (newCount))

// free the memory at a location in the chunks array.
#define FREE_ARRAY(type, pointer, oldCount) \
    reallocate(pointer, sizeof(type) * (oldCount), 0)

// reallocation boilerplate. rebound by macros.
// this function handles 4 cases:
// - if old_size is zero, and new_size is not zero, allocate memory.
// - if old_size is not zero, and new_size is zero, free the memory.
// - if old_size is not zero, and new_size is greater than old_size, grow the memory.
// - if old_size is not zero, and new_size is less than old_size, shrink the memory.
// at this point, i feel like this should just be multiple functions
void* reallocate(void* pointer, size_t old_size, size_t new_size);

#endif
