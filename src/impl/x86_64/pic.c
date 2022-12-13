#include "pic.h"

void pic_eoi(unsigned char irq)
{
	outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);
}

void picDisable() {
	__asm__ volatile (
		"mov $0xff, %al;"
		"out %al, $0xa1;"
		"out %al, $0x21;"
	);
	return;
}

void pic_init(int offset1, int offset2) {

    unsigned char a1, a2;
 
	a1 = inb(PIC1_DATA);                        // save masks
	a2 = inb(PIC2_DATA);
 
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
	outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);
	outb(0x21,0xfd);
    outb(0xa1,0xff);
	__asm__ volatile("sti");
}


void apicinit() {
	cpu_set_apic_base(cpu_get_apic_base());

	write_reg(0xF0, read_reg(0xF0) | 0x100);
}

uint32_t readAPIC(void *ioapicaddr, uint32_t reg) {
	uint32_t volatile *ioapic = (uint32_t volatile *)ioapicaddr;
	ioapic[0] = (reg & 0xff);
	return ioapic[4];
}

void writeAPIC(void *ioapicaddr, uint32_t reg, uint32_t value) {
	uint32_t volatile *ioapic = (uint32_t volatile *)ioapicaddr;
	ioapic[0] = (reg & 0xff);
	ioapic[4] = value;
}

uint32_t read_reg(uint32_t reg) {
    return *((volatile uint32_t *)(cpu_get_apic_base() + reg));
}

void cpuSetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi) {
    __asm__ volatile("wrmsr" : : "a"(lo), "d"(hi), "c"(msr));
}

void write_reg(uint32_t reg, uint32_t value) { 
	*((volatile uint32_t *)(cpu_get_apic_base() + reg)) = value; 
}

uintptr_t cpu_get_apic_base() {
	uint32_t edx, eax;
	cpuGetMSR(IA32_APIC_BASE_MSR, &eax, &edx);
	#ifdef __PHYSICAL_MEMORY_EXTENSION__
		return (eax & 0x0fffff0000) | ((edx & 0x0f) << 32);
	#else
		return (eax & 0x0fffff0000);
	#endif
}

void cpu_set_apic_base(uintptr_t apic) {
	uint32_t edx = 0;
	uint32_t eax = (apic & 0x0fffff0000) | IA32_APIC_BASE_MSR_ENABLE;

	#ifdef __PHYSICAL_MEMORY_EXTENSION__
		edx = (apic >> 32) & 0x0f;
	#endif
	cpuSetMSR(IA32_APIC_BASE_MSR, &eax, &edx);
}

void cpuGetMSR(uint32_t msr, uint32_t *lo, uint32_t *hi) {
    __asm__ volatile("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}