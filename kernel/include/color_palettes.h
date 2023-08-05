#pragma once

#include "colors.h"

const color_palette DEFAULT_COLOR_PALETTE =
{
	.black = 0x000000,
	.blue = 0x0000A8,
	.green = 0x00A800,
	.cyan = 0x00A8A8,
	.red = 0xA80000,
	.magenta = 0xA800A8,
	.brown = 0xA85700,
	.white = 0xA8A8A8,
	.gray = 0x575757,
	.light_blue = 0x5757FF,
	.light_green = 0x57FF57,
	.light_cyan = 0x57FFFF,
	.light_red = 0xFF5757,
	.light_magenta = 0xFF57FF,
	.yellow = 0xFFFF57,
	.bright_white = 0xFFFFFF
};

const color_palette DEFAULT_BRIGHT_COLOR_PALETTE =
{
	.black = 0xFFFFFF,
	.blue = 0x0000A8,
	.green = 0x00A800,
	.cyan = 0x00A8A8,
	.red = 0xA80000,
	.magenta = 0xA800A8,
	.brown = 0xA85700,
	.white = 0x575757,
	.gray = 0xA8A8A8,
	.light_blue = 0x5757FF,
	.light_green = 0x57FF57,
	.light_cyan = 0x57FFFF,
	.light_red = 0xFF5757,
	.light_magenta = 0xFF57FF,
	.yellow = 0xFFFF57,
	.bright_white = 0x000000
};

const color_palette MINIMALIST_COLOR_PALETTE =
{
	.black = 0x2D2727,
	.blue = 0x11009E,
	.green = 0x377D71,
	.cyan = 0x5B8FB9,
	.red = 0xCB1C8D,
	.magenta = 0x6527BE,
	.brown = 0xFBB454,
	.white = 0xECC9EE,
	.gray = 0x413543,
	.light_blue = 0x4942E4,
	.light_green = 0xB6EADA,
	.light_cyan = 0x46C2CB,
	.light_red = 0xFB2576,
	.light_magenta = 0x8F43EE,
	.yellow = 0xF0EB8D,
	.bright_white = 0xFDE2F3
};

const color_palette BLUEISH_COLOR_PALETTE =
{
	.black = 0x1f316f,
	.blue = 0x302BC6,
	.green = 0x553772,
	.cyan = 0x8f3b76,
	.red = 0xc7417b,
	.magenta = 0xFF578E,
	.brown = 0xC60F86,
	.white = 0xC16AC1,
	.gray = 0x354A99,
	.light_blue = 0x362CF3,
	.light_green = 0x7D559D,
	.light_cyan = 0xC457A6,
	.light_red = 0xFF5BA1,
	.light_magenta = 0xFF6CB0,
	.yellow = 0xFF1EB4,
	.bright_white = 0xFF91FF
};

const color_palette* COLOR_PALETTES[] =
{
	&DEFAULT_COLOR_PALETTE,
	&DEFAULT_BRIGHT_COLOR_PALETTE,
	&MINIMALIST_COLOR_PALETTE,
	&BLUEISH_COLOR_PALETTE
};
