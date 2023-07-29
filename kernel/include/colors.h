#pragma once

#include <stdint.h>
#include "io.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VGA_DAC_WRITE	0x3C8
#define VGA_DAC_DATA	0x3C9

#define RED(rgb)	((rgb >> 16) & 0xFF)
#define GREEN(rgb) ((rgb >> 8) & 0xFF)
#define BLUE(rgb) (rgb & 0xFF)
#define RGB(red, green, blue) (red << 16 | green << 8 | blue)

#define HSV_EDGE 65537
#define HUE_MAX (6 * HSV_EDGE)

void set_color_palette_rgb(uint8_t color_index, uint8_t r, uint8_t g, uint8_t b);

void set_color_palette(uint8_t color_index, uint32_t rgb);

uint32_t hsv2rgb(uint32_t hue, uint16_t saturation, uint8_t value);

#ifdef __cplusplus
}
#endif

