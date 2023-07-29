#include "debug.h"

#ifndef NDEBUG

void __debug(const char* str, size_t size)
{
	for (size_t i = 0; i < size; ++i)
	{
		char chr = str[i];
		if (chr == 0) return;
		debug_char(chr);
	}
}

#endif
