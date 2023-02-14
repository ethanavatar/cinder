#ifndef macros_h
#define macros_h

#include <stdio.h>

#define unused(x) { (void) x; fprintf(stderr, "WARN: Unused variable (Line %d)\n", __LINE__); }

#endif // macros_h
