#include "kdefs.h"

#include <stddef.h>
#include <stdint.h>

void* memset(void* ptr, int b, size_t n)
{
    uint8_t* ptr8 = (uint8_t*) ptr;
    for (size_t i = 0; i < n; ++i)
        ptr8[i] = (uint8_t) b;
    return ptr;
}
