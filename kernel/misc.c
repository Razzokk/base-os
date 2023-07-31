#include "misc.h"

#include <stddef.h>
#include <string.h>
#include <sys/io.h>

uint64_t rdtsc()
{
	uint64_t high;
	uint32_t low;
	asm volatile("rdtsc" : "=a"(low), "=d"(high));
	return (high << 32) | low;
}

void hw_cursor_disable()
{
	outb(0x3D4, 0x0A);
	// Fifth bit enables/disables cursor
	outb(0x3D5, 0x10);
}

void hw_cursor_enable(size_t cursor_start, size_t cursor_end)
{
	cursor_start &= 0x1F;
	cursor_end &= 0x1F;
	// Select 'Cursor Start Register', reset 'Cursor Disable' bit (bit 5) and set 'Cursor Scan Line Start'
	outb(0x3D4, 0x0A);
	outb(0x3D5, cursor_start);
	// Select 'Cursor End Register' and set 'Cursor Scan Line End'
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_end);
	// Select 'Cursor Start Register', reset 'Cursor Disable' bit (bit 5) and set 'Cursor Scan Line Start'
	outb(0x3D4, 0x14);
	outb(0x3D5, 10);
}

void hw_cursor_set_pos(size_t row, size_t col)
{
	uint16_t cursor_index = (uint16_t)(row * 80 + col);
	// Select 'Cursor Location High Register' and set 'Cursor Location High'
	outb(0x3D4, 0x0E);
	outb(0x3D5, cursor_index >> 8);
	// Select 'Cursor Location Low Register' and set 'Cursor Location Low'
	outb(0x3D4, 0x0F);
	outb(0x3D5, cursor_index & 0xFF);
}

void hw_cursor_get_pos(size_t* row, size_t* col)
{
	uint16_t cursor_index;
	// Select 'Cursor Location High Register' and read 'Cursor Location High'
	outb(0x3D4, 0x0E);
	cursor_index = inb(0x3D5) << 8;
	// Select 'Cursor Location Low Register' and read 'Cursor Location Low'
	outb(0x3D4, 0x0F);
	cursor_index |= inb(0x3D5);
	*row = cursor_index / 80;
	*col = cursor_index % 80;
}
