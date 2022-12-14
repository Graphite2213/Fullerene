#pragma once

#include <stdint.h>
#include <stddef.h>

#define IDT_MAX_DESCRIPTORS 256

#define GDT_OFFSET_KERNEL_CODE (0x01 * 0x08)

#define PAGING_PAGE_SIZE 0x200000
#define PAGING_PAGE_BOUNDARY 0x200000
#define PAGING_PAGE_ALIGNED __attribute__((aligned(PAGING_PAGE_SIZE)))

#define PAGING_KERNEL_OFFSET 0x0
#define PAGING_VIRTUAL_OFFSET 0x0

#define nullvptr -PAGING_VIRTUAL_OFFSET == NULL

typedef struct
{
    uint16_t base_low;
    uint16_t cs;
    uint8_t ist;
    uint8_t attributes;
    uint16_t base_mid;
    uint32_t base_high;
    uint32_t rsv0;
} __attribute__((packed)) idt_desc_t;

typedef enum boolean
{
    false = 0,
    true = !false
} bool;

typedef struct
{
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) idtr_t;

typedef struct
{
    uint64_t base;
    uint32_t limit;
    uint16_t access_byte;
    uint16_t flags;
} gdte_t;

typedef struct
{
    gdte_t null_descriptor;
    gdte_t kernel_code;
    gdte_t kernel_data;
    gdte_t user_code;
    gdte_t user_data;
    gdte_t tss;
} gdtt_t;


uint8_t idt_allocate_vector(void);
void idt_free_vector(uint8_t vector);
void idt_set_descriptor(uint8_t vector, uintptr_t isr, uint8_t flags);
void isr_exception_handler();
void idt_assemble(void);
void idt_init(void);
void initialize_keyboard(void);