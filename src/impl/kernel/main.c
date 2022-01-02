#include "ostdio.h"
#include "meman.h"

void kernel_main() {
    init_descriptor();
    print_clear();
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_BLACK);
    print_str("Welcome to FullereneOS!");
}
