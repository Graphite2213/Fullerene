#pragma once

#include <stdint.h>
#include <stddef.h>

struct GDT
{
    uint64_t base;
    uint64_t limit;
    uint64_t type;
    uint64_t something;
    uint64_t something2;
    uint64_t something3;
};

typedef struct GDT GDT_t;
void encodeGdtEntry();