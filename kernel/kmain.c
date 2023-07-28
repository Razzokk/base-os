#include <stdint.h>
#include <stdbool.h>
#include "kdefs.h"
#include "misc.h"
#include "terminal.h"

const char LOGO[] =
"___  ____ ____ ____    ____ ____\n"
"|__] |__| [__  |___    |  | [__ \n"
"|__] |  | ___] |___    |__| ___] v" XSTRINGIFY(OS_VERSION) "\n";

terminal_t terminal;

void kmain()
{
	term_init(&terminal);
	term_clear(&terminal);
	term_putstr(&terminal, LOGO, sizeof(LOGO));
}
