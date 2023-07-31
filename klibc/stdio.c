#include <stdio.h>

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define BUFFER_SIZE 1024
#define ULLTOA_BUFFER_SIZE (sizeof(unsigned long long) * 8 + 3)

#define min(a, b) ((a) < (b) ? (a) : (b))

static const char DIGITS[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

size_t ulltoa(unsigned long long value, char* buffer, int base, size_t writable)
{
	if (base < 2 || base > 36)
		return 0;

	size_t length = 0;
	char buf[ULLTOA_BUFFER_SIZE];

	// Parse the value to string (right to left)
	do
	{
		buf[length++] = DIGITS[value % base];
		value /= base;
	} while (value != 0);

	// write the string
	switch (base)
	{
		case 2:
			buf[length++] = 'b';
			buf[length++] = '0';
			break;
		case 8:
			buf[length++] = 'o';
			buf[length++] = '0';
			break;
		case 16:
			buf[length++] = 'x';
			buf[length++] = '0';
			break;
	}

	length = min(writable, length);

	for (size_t i = 0; i < length; ++i)
		buffer[i] = buf[length - i - 1];

	buffer[length] = 0;

	return length;
}

// Not standard conform at all, stops handling string after at most size bytes, no modifiers, flags, ...
int vsnprintf(char* str, size_t size, const char* format, va_list args)
{
	size_t length = 0;
	char c = *format;

	while (length < INT_MAX && c != 0)
	{
		if (c != '%')
		{
			if (length + 1 < size)
				str[length] = c;
			++length;
			c = *(++format);
			continue;
		}

		c = *(++format);
		if (c == 0) break;

		// Conversion

		long long int_val;
		unsigned long long uint_val;
		int sub_length;
		const char* string;
		size_t writable = (size == 0 || (length >= size)) ? 0 : size - length - 1;
		uint8_t base = 10;

		switch (c)
		{
			// Single characters
			case '%':
				if (writable > 0) str[length] = '%';
				sub_length = 1;
				c = *(++format);
				break;
			case 'c':
				if (writable > 0) str[length] = (char)va_arg(args, int);
				sub_length = 1;
				c = *(++format);
				break;

				// Signed integers
			case 'd':
			case 'i':
				int_val = va_arg(args, long long);
				if (int_val < 0)
				{
					uint_val = -int_val;
					if (writable > 0)
					{
						--writable;
						str[length] = '-';
						++length;
					}
				}
				else
				{
					uint_val = int_val;
				}
				sub_length = ulltoa(uint_val, str + length, base, writable);
				c = *(++format);
				break;

				// Unsigned integers
			case 'u':
				goto convert_unsigned;
			case 'B':
			case 'b':
				base = 2;
				goto convert_unsigned;
			case 'o':
				base = 8;
				goto convert_unsigned;
			case 'p':
			case 'X':
			case 'x':
				base = 16;

			convert_unsigned:
				uint_val = va_arg(args, unsigned long long);
				sub_length = ulltoa(uint_val, str + length, base, writable);
				c = *(++format);
				break;

				// String
			case 's':
				string = va_arg(args, const char*);
				sub_length = strlen(string);
				sub_length = min(sub_length, (int)writable);
				strncpy(str + length, string, sub_length);
				c = *(++format);
				break;

			default: sub_length = 0; break;
		}

		length += sub_length;
	}

	if (size != 0)
		str[min(size - 1, length)] = 0; // null byte

	return (int)length;
}

int sprintf(char* str, const char* format, ...)
{
	va_list args;

	va_start(args, format);
	int length = vsprintf(str, format, args);
	va_end(args);

	return length;
}

int snprintf(char* str, size_t size, const char* format, ...)
{
	va_list args;

	va_start(args, format);
	int length = vsnprintf(str, size, format, args);
	va_end(args);

	return length;
}

int vsprintf(char* str, const char* format, va_list ap)
{
	return vsnprintf(str, INT_MAX, format, ap);
}
