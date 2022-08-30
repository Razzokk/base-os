#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TEXT_BUF_ADDR 0xB8000
#define TEXT_BUF_COLS 80
#define TEXT_BUF_ROWS 25

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
    uint8_t fg: 4;
    uint8_t bg: 4;
} __attribute__((packed)) tbuf_color;

typedef struct
{
    uint8_t chr;
    tbuf_color color;
} tbuf_char;

extern tbuf_char* const text_buffer;

void tbuf_clear();

static inline tbuf_char tbuf_chr(uint8_t chr, vga_color fg, vga_color bg)
{
    return (tbuf_char) { chr, { fg, bg } };
}

static inline tbuf_char tbuf_at(size_t row, size_t col)
{
    return text_buffer[row * TEXT_BUF_COLS + col];
}

static inline tbuf_char tbuf_putchar(size_t row, size_t col, tbuf_char chr)
{
    return text_buffer[row * TEXT_BUF_COLS + col] = chr;
}

static inline tbuf_char tbuf_put(size_t row, size_t col, uint8_t chr, vga_color fg, vga_color bg)
{
    return tbuf_putchar(row, col, tbuf_chr(chr, fg, bg));
}

#ifdef __cplusplus
}
#endif
