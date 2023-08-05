#include "colors.h"

#include "terminal.h"

void set_color_palette_index_rbg(uint8_t color_index, uint8_t red, uint8_t green, uint8_t blue)
{
	// Dark Yellow, Brown - for some fucked up reason this is not properly handled as color 6 but needs also to be set as 20
	if (color_index == 6)
	{
		outb(VGA_DAC_WRITE, 20);
		outb(VGA_DAC_DATA, red / 4); // R (0-63)
		outb(VGA_DAC_DATA, green / 4); // G (0-63)
		outb(VGA_DAC_DATA, blue / 4); // B (0-63)
	}

	if (color_index >= 8)
		color_index += 48;

	// Set color index
	outb(VGA_DAC_WRITE, color_index);

	outb(VGA_DAC_DATA, red / 4); // R (0-63)
	outb(VGA_DAC_DATA, green / 4); // G (0-63)
	outb(VGA_DAC_DATA, blue / 4); // B (0-63)
}

void set_color_palette_index(uint8_t color_index, uint32_t rgb)
{
	set_color_palette_index_rbg(color_index, RED(rgb), GREEN(rgb), BLUE(rgb));
}

void set_color_palette(color_palette color_palette)
{
	set_color_palette_index(BLACK, color_palette.black);
	set_color_palette_index(BLUE, color_palette.blue);
	set_color_palette_index(GREEN, color_palette.green);
	set_color_palette_index(CYAN, color_palette.cyan);
	set_color_palette_index(RED, color_palette.red);
	set_color_palette_index(MAGENTA, color_palette.magenta);
	set_color_palette_index(BROWN, color_palette.brown);
	set_color_palette_index(WHITE, color_palette.white);
	set_color_palette_index(GRAY, color_palette.gray);
	set_color_palette_index(LIGHT_BLUE, color_palette.light_blue);
	set_color_palette_index(LIGHT_GREEN, color_palette.light_green);
	set_color_palette_index(LIGHT_CYAN, color_palette.light_cyan);
	set_color_palette_index(LIGHT_RED, color_palette.light_red);
	set_color_palette_index(LIGHT_MAGENTA, color_palette.light_magenta);
	set_color_palette_index(YELLOW, color_palette.yellow);
	set_color_palette_index(BRIGHT_WHITE, color_palette.bright_white);
}

uint32_t hsv2rgb(uint32_t hue, uint16_t saturation, uint8_t value)
{
	if (saturation == 0 || value == 0) return RGB(value, value, value);

	uint32_t delta = ((saturation * value) >> 16) + 1;
	uint32_t min = value - delta;
	uint8_t hue_index = hue / HSV_EDGE;

	uint32_t f = ((hue_index & 1) == 0) ?
			hue - HSV_EDGE * hue_index :
			HSV_EDGE * (hue_index + 1) - hue;

	uint32_t mid = ((f * delta) >> 16) + min;

	switch (hue_index)
	{
		case 1: return RGB(mid, value, min);
		case 2: return RGB(min, value, mid);
		case 3: return RGB(min, mid, value);
		case 4: return RGB(mid, min, value);
		case 5: return RGB(value, min, mid);
		default: return RGB(value, mid, min);
	}
}
