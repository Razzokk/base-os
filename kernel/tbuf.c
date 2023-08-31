#include "tbuf.h"

tbuf_char* text_buffer = NULL;
size_t text_buf_rows = 0;
size_t text_buf_cols = 0;

tbuf_char* tbuf_at(size_t row, size_t col)
{
	return text_buffer + row * text_buf_cols + col;
}

void tbuf_write(size_t row, size_t col, char chr, uint8_t fg, uint8_t bg)
{
	tbuf_char* c = tbuf_at(row, col);
	c->chr = chr;
	c->fg = fg;
	c->bg = bg;
}
