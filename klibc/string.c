#include <string.h>

#include <stdint.h>

int memcmp(const void* ptr1, const void* ptr2, size_t n)
{
	if (ptr1 == ptr2 || n == 0) return 0;

	const uint8_t* ptr1_8 = (const uint8_t*)ptr1;
	const uint8_t* ptr2_8 = (const uint8_t*)ptr2;
	uint8_t diff;

	do
	{
		diff = (*ptr1_8++) - (*ptr2_8++);
	} while (--n > 0 && diff == 0);

	return diff;
}

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

int strcmp(const char* str1, const char* str2)
{
	size_t length1 = strlen(str1);
	size_t length2 = strlen(str1);
	return strncmp(str1, str2, length1 < length2 ? length1 : length2);
}

int strncmp(const char* str1, const char* str2, size_t n)
{
	return memcmp((void*)str1, (void*)str2, n);
}

char* strcpy(char* dest, const char* source)
{
	return (char*) memcpy(dest, source, strlen(source));
}

char* strncpy(char* dest, const char* source, size_t n)
{
	return memcpy((void*)dest, (void*)source, n);
}

size_t strlen(const char* str)
{
	size_t length = 0;
	for (; str[length] != 0; ++length);
	return length;
}

size_t strnlen(const char* str, size_t n)
{
	size_t length = 0;
	for (; str[length] != 0 && length < n; ++length);
	return length;
}
