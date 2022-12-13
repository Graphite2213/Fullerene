#include <stdint.h>
#include <stddef.h>
#include "gdtmng.h"
#include "pic.h"
#include "stderr.h"

static PAGING_PAGE_ALIGNED
    idt_desc_t idt[IDT_MAX_DESCRIPTORS];

static idtr_t idtr;

static bool vectors[IDT_MAX_DESCRIPTORS];

extern uint64_t isr_stub_table[];

void idt_set_descriptor(uint8_t vector, uintptr_t isr, uint8_t flags)
{
    idt_desc_t *descriptor = &idt[vector];

    descriptor->base_low = isr & 0xFFFF;
    descriptor->cs = GDT_OFFSET_KERNEL_CODE;
    descriptor->ist = 0;
    descriptor->attributes = flags;
    descriptor->base_mid = (isr >> 16) & 0xFFFF;
    descriptor->base_high = (isr >> 32) & 0xFFFFFFFF;
    descriptor->rsv0 = 0;
}

void idt_assemble()
{
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_desc_t) * IDT_MAX_DESCRIPTORS - 1;

    for (uint8_t vector = 0; vector < IDT_CPU_EXCEPTION_COUNT + 3; vector++)
    {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
        vectors[vector] = true;
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__ volatile ("sti"); // set the interrupt flag
}

uint8_t idt_allocate_vector()
{
    for (unsigned int i = 0; i < IDT_MAX_DESCRIPTORS; i++)
    {
        if (!vectors[i])
        {
            vectors[i] = true;
            return (uint8_t)i;
        }
    }

    return 0;
}

void idt_free_vector(uint8_t vector)
{
    idt_set_descriptor(vector, 0, 0);
    vectors[vector] = false;
}

void initialize_keyboard()
{
}
