#include <stdint.h>
#include <stddef.h>
#include "ostdio.h"

void kerror(char* str) {
    print_clear();
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    print_str(str);
    __asm__ (
        "hlt"
    );
}
