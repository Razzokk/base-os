#pragma once

#include <stddef.h>
#include <stdint.h>

#define NUM_EXCEPTIONS	32
#define NUM_IRQS		16
#define NUM_INTERRUPTS	256

#define IDT_ALIGNMENT	0x1000

#define IDT_OFFSET_EXCEPTIONS	0
#define IDT_OFFSET_IRQS			NUM_EXCEPTIONS

#define GATE_TYPE_INTERRUPT	0x8E
#define GATE_TYPE_TRAP		0x8F

typedef struct gate_descriptor
{
	uint16_t isr_low;
	uint16_t segment_selector;
	uint8_t  ist;				// bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
	uint8_t  type_attributes;	// gate type, dpl, and p fields
	uint16_t isr_mid;
	uint32_t isr_high;
	uint32_t reserved;
} __attribute__((packed)) gate_descriptor;

typedef struct interrupt_frame
{
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} __attribute__((packed)) interrupt_frame;

typedef struct idt_ptr
{
	uint16_t limit;
	const gate_descriptor* base;
} __attribute__((packed)) idt_ptr;

extern gate_descriptor idt[NUM_INTERRUPTS];

static inline void disable_interrupts()
{
	asm volatile("cli");
}

static inline void enable_interrupts()
{
	asm volatile("sti");
}

void set_idt_desc(size_t index, void* isr, uint8_t segment_selector, uint8_t ist, uint8_t type_attributes);

void setup_interrupts();
