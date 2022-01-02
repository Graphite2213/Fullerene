#pragma once

void create_descriptor(uint32_t base, uint32_t limit, uint16_t flag);
void init_descriptor();
void PIC_sendEOI(unsigned char irq);