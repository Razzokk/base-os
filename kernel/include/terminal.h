#pragma once

#include <stddef.h>
#include <stdint.h>
#include "tbuf.h"

#ifdef __cplusplus
extern "C" {
#endif

// Only use this for string literals, else the outcome will probably be unexpected
#define term_putliteral(terminal, str) term_putstr(terminal, str, sizeof(str))

// Bright colors have the 4th bit set compared to the normal ones
typedef enum
{
	BLACK,          // 0b0000
	BLUE,           // 0b0001
	GREEN,          // 0b0010
	CYAN,           // 0b0011
	RED,            // 0b0100
	MAGENTA,        // 0b0101
	BROWN,          // 0b0110
	WHITE,          // 0b0111
	GRAY,           // 0b1000
	LIGHT_BLUE,     // 0b1001
	LIGHT_GREEN,    // 0b1010
	LIGHT_CYAN,     // 0b1011
	LIGHT_RED,      // 0b1100
	LIGHT_MAGENTA,  // 0b1101
	YELLOW,         // 0b1110
	BRIGHT_WHITE    // 0b1111
} vga_color;

typedef struct
{
	tbuf_char buffer[TEXT_BUF_ROWS * TEXT_BUF_COLS];
	uint8_t rows;
	uint8_t cols;
	uint8_t cursor_row;
	uint8_t cursor_col;
	uint8_t saved_cursor_row;
	uint8_t saved_cursor_col;
	vga_color foreground;
	vga_color background;
} terminal_t;

void term_init(terminal_t* terminal);

void term_clear_row(terminal_t* terminal, size_t row);

void term_clear(terminal_t* terminal);

void term_flush(terminal_t* terminal);

void term_write(terminal_t* terminal, size_t row, size_t col, char chr);

tbuf_char term_read(const terminal_t* terminal, size_t row, size_t col);

void term_scroll_up(terminal_t* terminal, size_t rows);

void term_newline(terminal_t* terminal);

void term_backspace(terminal_t* terminal);

void term_putchar(terminal_t* terminal, char chr);

void term_putstr(terminal_t* terminal, const char* str, size_t n);

void term_set_cursor_pos(terminal_t* terminal, size_t row, size_t col);

void term_save_cursor_pos(terminal_t* terminal);

void term_restore_cursor_pos(terminal_t* terminal);

void term_set_fg(terminal_t* terminal, vga_color color);

vga_color term_get_fg(const terminal_t* terminal);

void term_set_bg(terminal_t* terminal, vga_color color);

vga_color term_get_bg(const terminal_t* terminal);

#ifdef __cplusplus
}
#endif
