#include "debug.h"

#ifndef NDEBUG

#include <stdarg.h>
#include <stdio.h>

#define BUFFER_SIZE 128

void __debug(const char* str, size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		char chr = str[i];
		if (chr == 0) return;
		debug_char(chr);
	}
}

void __debugf(const char* format, ...)
{
	char buffer[BUFFER_SIZE];
	va_list args;
	va_start(args, format);
	size_t length = vsnprintf(buffer, BUFFER_SIZE, format, args);
	va_end(args);
	__debug(buffer, length);
}

#endif
