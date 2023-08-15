#include <stdnoreturn.h>
#include <stdbool.h>
#include "interrupts/interrupts.h"
#include "interrupts/pic.h"
#include "terminal.h"
#include "misc.h"
#include "debug.h"
#include "colors.h"
#include "rand.h"
#include "color_palettes.h"
#include "timer.h"

const char LOGO[] =
"  ______   ______   ______    \n"
" /\\  == \\ /\\  __ \\ /\\  ___\\   \n"
" \\ \\  __< \\ \\ \\/\\ \\\\ \\___  \\  \n"
"  \\ \\_____\\\\ \\_____\\\\/\\_____\\ \n"
"   \\/_____/ \\/_____/ \\/_____/ v" xstringify(OS_VERSION) "\n\n";

terminal_t terminal;

extern void* kernel_start_address;
extern void* kernel_end_address;

void print_colors(void)
{
	term_putliteral(&terminal, "Available colors:\n\n");
	vga_color current_bg = term_get_bg(&terminal);

	for (uint8_t color = 0; color < 8; ++color)
	{
		term_set_bg(&terminal, color);
		term_putliteral(&terminal, "  ");
	}

	term_putliteral(&terminal, "\n");

	for (uint8_t color = 8; color < 16; ++color)
	{
		term_set_bg(&terminal, color);
		term_putliteral(&terminal, "  ");
	}

	term_set_bg(&terminal, current_bg);
}

noreturn void kmain(void)
{
	setup_interrupts();
	enable_interrupts();
	seed_rand(rdtsc());

	term_init(&terminal);
	term_clear(&terminal);

	set_color_palette(MINIMALIST_COLOR_PALETTE);
	term_set_fg(&terminal, BLACK);
	term_set_fg(&terminal, LIGHT_GREEN);
	term_putstr(&terminal, LOGO, sizeof(LOGO));

	term_putchar(&terminal, '\n');
	print_colors();

	debugf("[%s]: print some useful information for %s here\n", "\x1b[33mDEBUG\x1b[m", "debugging");

	term_putliteral(&terminal, "\n\nHere a random number: ");
	term_set_fg(&terminal, RED);
	term_printf(&terminal, "%d", rand());
	term_set_fg(&terminal, LIGHT_GREEN);

	term_putliteral(&terminal, "\n\nWrite some stuff:\n");

	pic_enable_irq(1); // enable keyboard interrupts
	init_timer(20);

	// let's cause a page-fault:
//	*(int*)(0x123456789) = 42;

	// We don't return from kmain
	while (true);
}
