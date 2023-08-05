#pragma once

#include <stdint.h>
#include <sys/io.h>

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


typedef struct color_palette
{
	uint32_t black;
	uint32_t blue;
	uint32_t green;
	uint32_t cyan;
	uint32_t red;
	uint32_t magenta;
	uint32_t brown;
	uint32_t white;
	uint32_t gray;
	uint32_t light_blue;
	uint32_t light_green;
	uint32_t light_cyan;
	uint32_t light_red;
	uint32_t light_magenta;
	uint32_t yellow;
	uint32_t bright_white;
} color_palette;

void set_color_palette_index_rgb(uint8_t color_index, uint8_t r, uint8_t g, uint8_t b);

void set_color_palette_index(uint8_t color_index, uint32_t rgb);

void set_color_palette(color_palette color_palette);

uint32_t hsv2rgb(uint32_t hue, uint16_t saturation, uint8_t value);

#ifdef __cplusplus
}
#endif

