#include "common.h"
#include <stdio.h>
#include "scanner.h"
#include "macros.h"

int32_t main(const int32_t argc, const char* argv[]) {
    unused(argc);
    unused(argv);

    init_scanner("x := 0;");
    return 0;
}
