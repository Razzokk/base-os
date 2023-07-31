#include "terminal.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "misc.h"

#define BUFFER_SIZE 128

void term_init(terminal_t* terminal)
{
	terminal->rows = TEXT_BUF_ROWS;
	terminal->cols = TEXT_BUF_COLS;
	terminal->cursor_row = 0;
	terminal->cursor_col = 0;
	terminal->saved_cursor_row = 0;
	terminal->saved_cursor_col = 0;
	terminal->foreground = LIGHT_GREEN;
	terminal->background = BLACK;
	memset(terminal->buffer, 0, terminal->rows * terminal->cols * sizeof(tbuf_char));
	hw_cursor_enable(0, 15);
	hw_cursor_set_pos(0, 0);
}

void term_clear_row(terminal_t* terminal, size_t row)
{
	for (size_t col = 0; col < terminal->cols; ++col)
		term_write(terminal, row, col, ' ');
}

void term_clear(terminal_t* terminal)
{
	for (size_t row = 0; row < terminal->rows; ++row)
		term_clear_row(terminal, row);
}

void term_flush(terminal_t* terminal)
{
	memcpy(text_buffer, terminal->buffer, sizeof(tbuf_char) * terminal->rows * terminal->cols);
}

void term_write(terminal_t* terminal, size_t row, size_t col, char chr)
{
	tbuf_char* c = terminal->buffer + row * terminal->cols + col;
	c->chr = chr;
	c->fg = terminal->foreground;
	c->bg = terminal->background;
	tbuf_write(row, col, chr, terminal->foreground, terminal->background);
}

void term_scroll_up(terminal_t* terminal, size_t rows)
{
	if (rows > terminal->rows) rows = terminal->rows;
	memmove(terminal->buffer, terminal->buffer + rows * terminal->cols, (terminal->rows - rows) * terminal->cols * sizeof(tbuf_char));
	term_flush(terminal);

	for (size_t row = (terminal->rows - rows); row < terminal->rows; ++row)
		term_clear_row(terminal, row);
}

void term_newline(terminal_t* terminal)
{
	terminal->cursor_col = 0;
	if (terminal->cursor_row == terminal->rows - 1)
		term_scroll_up(terminal, 1);
	else
		++terminal->cursor_row;

	hw_cursor_set_pos(terminal->cursor_row, terminal->cursor_col);
}

void term_backspace(terminal_t* terminal)
{
	if (terminal->cursor_col == 0) return;
	--terminal->cursor_col;
	term_write(terminal, terminal->cursor_row, terminal->cursor_col, ' ');
	hw_cursor_set_pos(terminal->cursor_row, terminal->cursor_col);
}

void term_putchar(terminal_t* terminal, char chr)
{
	if (chr == '\n')
	{
		term_newline(terminal);
		return;
	}

	if (chr == '\b')
	{
		term_backspace(terminal);
		return;
	}

	term_write(terminal, terminal->cursor_row, terminal->cursor_col, chr);
	++terminal->cursor_col;

	if (terminal->cursor_col >= terminal->cols)
		term_newline(terminal);

	hw_cursor_set_pos(terminal->cursor_row, terminal->cursor_col);
}

void term_putstr(terminal_t* terminal, const char* str, size_t n)
{
	for (size_t i = 0; i < n; ++i)
	{
		char chr = str[i];
		if (chr == 0) return;
		term_putchar(terminal, chr);
	}
}

void term_printf(terminal_t* terminal, const char* format, ...)
{
	char buffer[BUFFER_SIZE];
	va_list args;
	va_start(args, format);
	size_t length = vsnprintf(buffer, BUFFER_SIZE, format, args);
	va_end(args);
	term_putstr(terminal, buffer, length);
}

void term_set_cursor_pos(terminal_t* terminal, size_t row, size_t col)
{
	terminal->cursor_row = row;
	terminal->cursor_col = col;
}

void term_save_cursor_pos(terminal_t* terminal)
{
	terminal->saved_cursor_row = terminal->cursor_row;
	terminal->saved_cursor_col = terminal->cursor_col;
}

void term_restore_cursor_pos(terminal_t* terminal)
{
	term_set_cursor_pos(terminal, terminal->saved_cursor_row, terminal->saved_cursor_col);
}

void term_set_fg(terminal_t* terminal, vga_color color)
{
	terminal->foreground = color;
}

vga_color term_get_fg(const terminal_t* terminal)
{
	return terminal->foreground;
}

void term_set_bg(terminal_t* terminal, vga_color color)
{
	terminal->background = color;
}

vga_color term_get_bg(const terminal_t* terminal)
{
	return terminal->background;
}
