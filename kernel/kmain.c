#include <stdnoreturn.h>
#include <stdbool.h>
#include "kdefs.h"
#include "interrupts/interrupts.h"
#include "interrupts/pic.h"
#include "terminal.h"
#include "misc.h"
#include "debug.h"
#include "colors.h"
#include "rand.h"

const char LOGO[] =
"  ______   ______   ______    \n"
" /\\  == \\ /\\  __ \\ /\\  ___\\   \n"
" \\ \\  __< \\ \\ \\/\\ \\\\ \\___  \\  \n"
"  \\ \\_____\\\\ \\_____\\\\/\\_____\\ \n"
"   \\/_____/ \\/_____/ \\/_____/ v" XSTRINGIFY(OS_VERSION) "\n\n";

terminal_t terminal;
char buffer[ULLTOA_BUF_SIZE];

void init_color_palette()
{
	set_color_palette(BLACK, 0x2D2727);
	set_color_palette(BLUE, 0x11009E);
	set_color_palette(GREEN, 0x377D71);
	set_color_palette(CYAN, 0x5B8FB9);
	set_color_palette(RED, 0xCB1C8D);
	set_color_palette(MAGENTA, 0x6527BE);
	set_color_palette(BROWN, 0xFBB454);
	set_color_palette(WHITE, 0xECC9EE);
	set_color_palette(GRAY, 0x413543);
	set_color_palette(LIGHT_BLUE, 0x4942E4);
	set_color_palette(LIGHT_GREEN, 0xB6EADA);
	set_color_palette(LIGHT_CYAN, 0x46C2CB);
	set_color_palette(LIGHT_RED, 0xFB2576);
	set_color_palette(LIGHT_MAGENTA, 0x8F43EE);
	set_color_palette(YELLOW, 0xF0EB8D);
	set_color_palette(BRIGHT_WHITE, 0xFDE2F3);
}

void print_colors()
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

noreturn void kmain()
{
	setup_interrupts();
	enable_interrupts();
	seed_rand(rdtsc());

	term_init(&terminal);
	term_clear(&terminal);

	init_color_palette();
	term_set_fg(&terminal, BLACK);
	term_set_fg(&terminal, LIGHT_GREEN);
	term_putstr(&terminal, LOGO, sizeof(LOGO));

	term_putchar(&terminal, '\n');
	print_colors();

	debug_literal("[\x1b[33mDEBUG\x1b[m]: print some useful information for debugging here\n");

	pic_enable_irq(0); // enable timer interrupts

	term_putliteral(&terminal, "\n\nHere a random number: ");
	size_t len = ulltoa(rand(), buffer, 10);
	term_set_fg(&terminal, RED);
	term_putstr(&terminal, buffer, len);
	term_set_fg(&terminal, LIGHT_GREEN);

	// let's cause a page-fault:
//	*(int*)(0x123456789) = 42;

	// We don't return from kmain
	while (true);
}
