#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* dest, const void* source, size_t n);

void* memmove(void* dest, const void* source, size_t n);

void* memset(void* ptr, int value, size_t n);

#ifdef __cplusplus
}
#endif
