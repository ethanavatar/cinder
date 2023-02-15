#include <stdlib.h>

#include "macros.h"
#include "memory.h"

void* reallocate(void* pointer, size_t old_size, size_t new_size) {
    unused(old_size);

    // if new_size is zero, free the memory.
    if (new_size == 0) {
        free(pointer);
        return NULL;
    }

    // new_size is not zero, so we need to allocate memory.

    // resize the memory
    void* result = realloc(pointer, new_size);

    // ran out of memory. exit the program.
    if (result == NULL) exit(1);

    // return the the pointer to the new memory
    return result;
}
