#include "misc.h"

#include <stddef.h>
#include <string.h>

static const char DIGITS[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

size_t ulltoa(unsigned long long value, char* buffer, int base)
{
	if (base < 2 || base > 36)
		return 0;

	size_t number_digits = 0;
	size_t length = 0;
	char buf[ULLTOA_BUF_SIZE];

	// Parse the value to string (right to left)
	do
	{
		buf[number_digits++] = DIGITS[value % base];
		value /= base;
	} while (value != 0);

	// write the string
	switch (base)
	{
		case 2:
			buffer[length++] = '0';
			buffer[length++] = 'b';
			break;
		case 8:
			buffer[length++] = '0';
			buffer[length++] = 'o';
			break;
		case 16:
			buffer[length++] = '0';
			buffer[length++] = 'x';
			break;
	}

	for (size_t i = 0; i < number_digits; ++i)
		buffer[length++] = buf[number_digits - i - 1];

	buffer[length] = 0;

	return length;
}

uint64_t rdtsc()
{
	uint64_t high;
	uint32_t low;
	asm volatile("rdtsc" : "=a"(low), "=d"(high));
	return (high << 32) | low;
}
