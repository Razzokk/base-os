#include "misc.h"

#include <stddef.h>
#include <string.h>

static const char DIGITS[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

size_t ulltoa(unsigned long long value, char* buffer, int base)
{
	if (base < 2 || base > 36)
		return 0;

	size_t length = 0;

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

	// Parse the value to string (right to left)
	do
	{
		buffer[length++] = DIGITS[value % base];
		value /= base;
	} while (value != 0);

	buffer[length] = 0;

	return length;
}
