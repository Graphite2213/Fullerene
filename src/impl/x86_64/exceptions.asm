extern isr_exception_handler
extern isr_divide_by_zero_handler
extern isr_keyboard_handler
extern isr_invalid_op_handler
extern isr_fault_handler

%macro isr_err_stub 1
isr_stub_%+%1:
    call isr_fault_handler
    iretq
%endmacro
; if writing for 64-bit, use iretq instead
%macro isr_no_err_stub 1
isr_stub_%+%1:
    call isr_exception_handler
    iretq
%endmacro

%macro div_by_zero 1
isr_stub_%+%1:
    call isr_divide_by_zero_handler
    iretq
%endmacro

%macro isr_keyboard 1
isr_stub_%+%1:
    call isr_keyboard_handler
    iretq
%endmacro

%macro isr_op 1
isr_stub_%+%1:
    call isr_invalid_op_handler
    iretq
%endmacro

%macro isr_gen 1
isr_gen_%+%1:
    mov dword [0xb8000], 0x2f4b2f4f
    iretq
%endmacro

div_by_zero 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_op 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31
isr_keyboard 32
isr_keyboard 33
isr_keyboard 34
isr_keyboard 35
isr_keyboard 36
isr_keyboard 37
isr_keyboard 38
isr_keyboard 39
isr_keyboard 40
isr_keyboard 41
isr_keyboard 42
isr_keyboard 43
isr_keyboard 44
isr_keyboard 45
isr_keyboard 46
isr_keyboard 47


global isr_stub_table
isr_stub_table:
%assign i 0 
%rep    32 
    dq isr_stub_%+i ; use DQ instead if targeting 64-bit
%assign i i+1 
%endrep