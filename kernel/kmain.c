#include <stdint.h>
#include <stdbool.h>
#include "kdefs.h"
#include "tbuf.h"

const char LOGO[] =
"___  ____ ____ ____    ____ ____\n"
"|__] |__| [__  |___    |  | [__ \n"
"|__] |  | ___] |___    |__| ___]\n";

void print_logo()
{
	const char *logo_ptr = LOGO;
	char c = *logo_ptr;
	uint8_t row = 0;
	uint8_t col = 0;

	while (c != 0)
	{
		if (c == '\n')
		{
			++row;
			col = 0;
		}
		else
		{
			tbuf_put(row, col, c, LIGHT_GREEN, BLACK);
			++col;
		}
		++logo_ptr;
		c = *logo_ptr;
	}
}

void kmain()
{
	tbuf_clear();
	print_logo();
}
