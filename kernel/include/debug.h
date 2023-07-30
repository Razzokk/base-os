#pragma once

#include <stddef.h>
#include <sys/io.h>

#ifdef __cplusplus
extern "C" {
#endif

#define COM1_PORT 0x3F8 // (can) used for serial log
#define DEBUG_PORT 0xE9

#ifndef NDEBUG

void __debug(const char* str, size_t size);

#define debug_char(chr) outb(DEBUG_PORT, chr)
#define debug(str, size) __debug(str, size)
#define debug_literal(str) __debug(str, sizeof(str))

#else

#define debug_char(chr) ((void)0)
#define debug(str, size) ((void)0)
#define debug_literal(str) ((void)0)

#endif

#ifdef __cplusplus
}
#endif

