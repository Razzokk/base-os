#include "tbuf.h"

tbuf_char* const text_buffer = (tbuf_char* const) TEXT_BUF_ADDR;

tbuf_char* tbuf_at(size_t row, size_t col)
{
	return text_buffer + row * TEXT_BUF_COLS + col;
}

void tbuf_write(size_t row, size_t col, char chr, uint8_t fg, uint8_t bg)
{
	tbuf_char* c = tbuf_at(row, col);
	c->chr = chr;
	c->fg = fg;
	c->bg = bg;
}
