#include <stdlib.h>

#include "macros.h"
#include "memory.h"
#include "vm.h"

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

void free_object(Obj* object) {
    switch (object->type) {
    case OBJ_FUNCTION: {
        ObjFunction* function = (ObjFunction*)object;
        free_chunk(&function->chunk);
        FREE(ObjFunction, object);
        break;
    }
    case OBJ_NATIVE:
      FREE(ObjNative, object);
      break;
    case OBJ_STRING: {
        ObjString* string = (ObjString*)object;
        FREE_ARRAY(char, string->chars, string->length + 1);
        FREE(ObjString, object);
        break;
    }
    default:
        break;
    }
}

void free_objects() {
    Obj* object = vm.objects;
    while (object != NULL) {
        Obj* next = object->next;
        free_object(object);
        object = next;
    }
}
