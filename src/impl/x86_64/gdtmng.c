#include <stdint.h>
#include <stddef.h>
#include "gdtmng.h"
#include "pic.h"
#include "stderr.h"

static PAGING_PAGE_ALIGNED
    idt_desc_t idt[IDT_MAX_DESCRIPTORS];

static idtr_t idtr;
static gdtt_t gdtr;

static bool vectors[IDT_MAX_DESCRIPTORS];

extern uint64_t isr_stub_table[];

void gdt_assemble()
{
    gdte_t null_entry;
    null_entry.base = 0;
    null_entry.access_byte = 0x00;
    null_entry.limit = 0x00000000;
    null_entry.flags = 0x0;

    gdte_t kernel_code;
    kernel_code.base = 0;
    kernel_code.access_byte = 0x9A;
    kernel_code.limit = 0xFFFFF;
    kernel_code.flags = 0xA;

    gdte_t kernel_data;
    kernel_data.base = 0;
    kernel_data.access_byte = 0x92;
    kernel_data.limit = 0xFFFFF;
    kernel_data.flags = 0xC;

    gdte_t user_code;
    user_code.base = 0;
    user_code.access_byte = 0xFA;
    user_code.limit = 0xFFFFF;
    user_code.flags = 0xA;

    gdte_t user_data;
    user_data.base = 0;
    user_data.access_byte = 0xF2;
    user_data.limit = 0x00000000;
    user_data.flags = 0xC;

    gdte_t tss;
    tss.base = 0;
    tss.access_byte = 0x64;
    tss.limit = 0x64;
    tss.flags = 0x0;


    gdtr.null_descriptor = null_entry;
    gdtr.kernel_code = kernel_code;
    gdtr.kernel_data = kernel_data;
    gdtr.user_code = user_code;
    gdtr.user_data = user_data;
    gdtr.tss = tss;

    __asm__ volatile ("lgdt %0" : : "m"(gdtr)); // load the new IDT
}

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

    for (uint8_t vector = 0; vector < 35; vector++)
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
