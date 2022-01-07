#include <stdint.h>
#include <stddef.h>
#include "ostdio.h"

void safe_reboot() {
    
}

void kerror(char* str) {
    print_clear();
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    print_str(str);
    __asm__ (
        "hlt"
    );
}

void ferror() {
    print_clear();
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_RED);
    print_str("A fatal error has occured");
    print_newline();
    print_str("Your device will reboot shortly");
    safe_reboot();
}