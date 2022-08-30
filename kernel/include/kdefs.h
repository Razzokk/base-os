#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline void io_delay()
{
    asm volatile("outb %al, $0x80");
}

void* memset(void* ptr, int b, size_t n);

#ifdef __cplusplus
}
#endif
