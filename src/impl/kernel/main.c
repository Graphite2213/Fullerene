#include "ostdio.h"
#include "gdtmng.h"
#include "picc.h"

void kernel_main() {
    print_clear();
    print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
    idt_assemble();
    pic_init(0x20, 0x28);
    print_str("Deez Nuts");
}
