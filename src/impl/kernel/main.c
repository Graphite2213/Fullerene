#include "ostdio.h"
#include "gdtmng.h"

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
    idt_assemble();
    print_str("Deez Nuts");
}
