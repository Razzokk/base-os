#pragma once

#include <stddef.h>
#include <stdint.h>
#include "memory.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	uint8_t chr;
	uint8_t fg: 4;
	uint8_t bg: 4;
} __attribute__((packed)) tbuf_char;

extern tbuf_char* text_buffer;
extern size_t text_buf_rows;
extern size_t text_buf_cols;

tbuf_char* tbuf_at(size_t row, size_t col);

void tbuf_write(size_t row, size_t col, char chr, uint8_t fg, uint8_t bg);

#ifdef __cplusplus
}
#endif
