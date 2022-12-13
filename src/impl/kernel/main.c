#include "stdio.h"
#include "gdtmng.h"
#include "pic.h"

void kernel_main() 
{
    print_clear();
    print_set_color(PRINT_COLOR_GREEN, PRINT_COLOR_BLACK);
    idt_assemble();
    pic_init(0x20, 0x28);
    while(1);
}
