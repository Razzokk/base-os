#pragma once

#include <stddef.h>
#include <stdint.h>
#include "memory.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEXT_BUF_ADDR P2V(0xB8000)
#define TEXT_BUF_ROWS 25
#define TEXT_BUF_COLS 80

typedef struct
{
	uint8_t chr;
	uint8_t fg: 4;
	uint8_t bg: 4;
} __attribute__((packed)) tbuf_char;

extern tbuf_char* const text_buffer;

tbuf_char* tbuf_at(size_t row, size_t col);

void tbuf_write(size_t row, size_t col, char chr, uint8_t fg, uint8_t bg);

#ifdef __cplusplus
}
#endif
