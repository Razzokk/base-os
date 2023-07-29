#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STRINGIFY(x) #x
#define XSTRINGIFY(x) STRINGIFY(x)

#define ULLTOA_BUF_SIZE (sizeof(unsigned long long) * 8 + 3)

// Returns the length of the resulting string, excluding the null byte
size_t ulltoa(unsigned long long value, char* buffer, int base);

uint64_t rdtsc();

#ifdef __cplusplus
}
#endif
