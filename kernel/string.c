#include <string.h>

#include <stdint.h>

void* memcpy(void* dest, const void* source, size_t n)
{
	if (dest == source || n == 0) return dest;

	const uint8_t* s8 = (const uint8_t*) source;
	uint8_t* d8 = (uint8_t*) dest;

	for (size_t i = 0; i < n; ++i)
		d8[i] = s8[i];

	return dest;
}

void* memmove(void* dest, const void* source, size_t n)
{
	if (dest == source || n == 0) return dest;

	if (source > dest)
		return memcpy(dest, source, n);

	const uint8_t* s8 = (const uint8_t*) source;
	uint8_t* d8 = (uint8_t*) dest;

	for (size_t i = 0; i < n; ++i)
		d8[n - i - 1] = s8[n - i - 1];

	return dest;
}

void* memset(void* ptr, int value, size_t n)
{
	uint8_t* ptr8 = (uint8_t*) ptr;

	for (size_t i = 0; i < n; ++i)
		ptr8[i] = (uint8_t) value;

	return ptr;
}
