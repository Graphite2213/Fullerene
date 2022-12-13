#include <stdint.h>
#include <stddef.h>
#include "stdio.h"
#include "pic.h"

void isr_divide_by_zero_handler()
{
    kerror("ERR_DIV_ZERO");
}

void isr_exception_handler()
{
    kerror("ERR_GEN");
}

void isr_invalid_op_handler()
{
    kerror("ERR_OP");
}


void isr_fault_handler()
{
    kerror("ERR_DEATH");
}


void isr_keyboard_handler()
{
    pic_eoi(0x21);
    kerror("Hello");
}


void kerror(char* str) 
{
    print_set_color(PRINT_COLOR_RED, PRINT_COLOR_BLACK);
    print_str(str);
    __asm__ (
        "hlt"
    );
}

void ferror() 
{
    print_clear();
    print_set_color(PRINT_COLOR_WHITE, PRINT_COLOR_RED);
    print_str("A fatal error has occured");
    print_newline();
    print_str("Your device will reboot shortly");
}