#pragma once

#include <stdint.h>
#include <stddef.h>

#define IDT_MAX_DESCRIPTORS 256
#define IDT_CPU_EXCEPTION_COUNT 32

#define IDT_DESCRIPTOR_X16_INTERRUPT 0x06
#define IDT_DESCRIPTOR_X16_TRAP 0x07
#define IDT_DESCRIPTOR_X32_TASK 0x05
#define IDT_DESCRIPTOR_X32_INTERRUPT 0x0E
#define IDT_DESCRIPTOR_X32_TRAP 0x0F
#define IDT_DESCRIPTOR_RING1 0x40
#define IDT_DESCRIPTOR_RING2 0x20
#define IDT_DESCRIPTOR_RING3 0x60
#define TSS_IST_EXCEPTION 001
#define TSS_IST_ROUTINE 002
#define IDT_DESCRIPTOR_PRESENT 0x80

#define IDT_DESCRIPTOR_EXCEPTION (IDT_DESCRIPTOR_X32_INTERRUPT | IDT_DESCRIPTOR_PRESENT)
#define IDT_DESCRIPTOR_EXTERNAL (IDT_DESCRIPTOR_X32_INTERRUPT | IDT_DESCRIPTOR_PRESENT)
#define IDT_DESCRIPTOR_CALL (IDT_DESCRIPTOR_X32_INTERRUPT | IDT_DESCRIPTOR_PRESENT | IDT_DESCRIPTOR_RING3)

#define GDT_MAX_DESCRIPTORS 0x2000
#define GDT_DESCRIPTOR_SIZE 0x08

#define GDT_DESCRIPTOR_ACCESS 0x01
#define GDT_DESCRIPTOR_READWRITE 0x02
#define GDT_DESCRIPTOR_DC 0x04
#define GDT_DESCRIPTOR_EXECUTABLE 0x08
#define GDT_DESCRIPTOR_CODE_DATA 0x10
#define GDT_DESCRIPTOR_DPL 0x60
#define GDT_DESCRIPTOR_PRESENT 0x80

#define GDT_GRANULARITY_OS 0x10
#define GDT_GRANULARITY_X64 0x20
#define GDT_GRANULARITY_X32 0x40
#define GDT_GRANULARITY_4K 0x80

#define GDT_BASIC_DESCRIPTOR (GDT_DESCRIPTOR_PRESENT | GDT_DESCRIPTOR_READWRITE | GDT_DESCRIPTOR_CODE_DATA)
#define GDT_BASIC_GRANULARITY (GDT_GRANULARITY_X64 | GDT_GRANULARITY_4K)

#define GDT_OFFSET_KERNEL_CODE (0x01 * 0x08)
#define GDT_OFFSET_KERNEL_DATA (0x02 * 0x08)
#define GDT_OFFSET_USER_DATA (0x03 * 0x08)
#define GDT_OFFSET_USER_CODE (0x04 * 0x08)

#define PAGING_FLAG_PRESENT 0x001
#define PAGING_FLAG_WRITE 0x002
#define PAGING_FLAG_USER 0x004
#define PAGING_FLAG_WRITE_THROUGH 0x008
#define PAGING_FLAG_CACHE_DISABLE 0x010
#define PAGING_FLAG_ACCESSED 0x020
#define PAGING_FLAG_LARGER_PAGES 0x040
#define PAGING_FLAG_OS_AVAILABLE 0xE00
#define PAGING_FLAG_NO_EXECUTE (1 << 63)

#define PAGING_FLAGS_KERNEL_PAGE (PAGING_FLAG_PRESENT | PAGING_FLAG_WRITE)
#define PAGING_FLAGS_USER_PAGE (PAGING_FLAG_PRESENT | PAGING_FLAG_WRITE | PAGING_FLAG_USER)

#define PAGING_PAGE_SIZE 0x1000
#define PAGING_PAGE_BOUNDARY 0x1000
#define PAGING_PAGE_ALIGNED __attribute__((aligned(PAGING_PAGE_SIZE)))

#define PAGING_KERNEL_OFFSET 0xffffffff80000000
#define PAGING_VIRTUAL_OFFSET 0xffff800000000000

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

void idt_reload(idtr_t *idtr);
uint8_t idt_allocate_vector(void);
void idt_free_vector(uint8_t vector);
void idt_set_descriptor(uint8_t vector, uintptr_t isr, uint8_t flags, uint8_t ist);
void isr_exception_handler();
void idt_assemble(void);
void idt_init(void);