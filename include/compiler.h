#ifndef cinder_compiler_h
#define cinder_compiler_h

#include "vm.h"

ObjFunction* compile(const char* source);

void mark_compiler_roots();

#endif
