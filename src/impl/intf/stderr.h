#pragma once

#include <stdint.h>
#include <stddef.h>

void kerror(char* str);

void ferror();

void isr_exception_handler();
void isr_divide_by_zero_handler();
void isr_keyboard_handler();
void isr_invalid_op_handler();
void isr_fault_handler();