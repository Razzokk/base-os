#pragma once

#include <stddef.h>
#include <sys/io.h>

#ifdef __cplusplus
extern "C" {
#endif

#define COM1_PORT 0x3F8 // (can) used for serial log
#define DEBUG_PORT 0xE9

#define DBG_GREEN "[\x1b[32mDEBUG\x1b[m]: "
#define DBG_YELLOW "[\x1b[33mDEBUG\x1b[m]: "
#define DBG_RED "[\x1b[31mDEBUG\x1b[m]: "

#ifndef NDEBUG

void __debug(const char* str, size_t size);
void __debugf(const char* format, ...);

#define debug_char(chr) outb(DEBUG_PORT, chr)
#define debug(str, size) __debug(str, size)
#define debug_literal(str) __debug(str, sizeof(str))
#define debugf(format, ...) __debugf(format, __VA_ARGS__)

#else

#define debug_char(chr) ((void)0)
#define debug(str, size) ((void)0)
#define debug_literal(str) ((void)0)
#define debugf(format, ...) ((void)0)

#endif

#ifdef __cplusplus
}
#endif

