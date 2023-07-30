#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Really rough approx.
// io_delay has ~ 1-4 microsecond delay
static inline void io_delay()
{
	asm volatile("outb %al, $0x80");
}

static inline uint8_t inb(uint16_t port)
{
	uint8_t value;
	asm volatile("inb %w1, %b0" : "=a"(value) : "Nd"(port));
	return value;
}

static inline uint16_t inw(uint16_t port)
{
	uint16_t value;
	asm volatile("inw %w1, %w0" : "=a"(value) : "Nd"(port));
	return value;
}

static inline uint32_t inl(uint16_t port)
{
	uint32_t value;
	asm volatile("inl %w1, %0" : "=a"(value) : "Nd"(port));
	return value;
}

static inline void outb(uint16_t port, uint8_t value)
{
	asm volatile("outb %b0, %w1" :: "a"(value), "Nd"(port));
}

static inline void outw(uint16_t port, uint16_t value)
{
	asm volatile("outw %w0, %w1" :: "a"(value), "Nd"(port));
}

static inline void outl(uint16_t port, uint32_t value)
{
	asm volatile("outl %0, %w1" :: "a"(value), "Nd"(port));
}

static inline uint8_t inb_p(uint16_t port)
{
	uint8_t value = inb(port);
	io_delay();
	return value;
}

static inline uint16_t inw_p(uint16_t port)
{
	uint16_t value = inw(port);
	io_delay();
	return value;
}

static inline uint32_t inl_p(uint16_t port)
{
	uint32_t value = inl(port);
	io_delay();
	return value;
}

static inline void outb_p(uint16_t port, uint8_t value)
{
	outb(port, value);
	io_delay();
}

static inline void outw_p(uint16_t port, uint16_t value)
{
	outw(port, value);
	io_delay();
}

static inline void outl_p(uint16_t port, uint32_t value)
{
	outl(port, value);
	io_delay();
}

#ifdef __cplusplus
}
#endif
