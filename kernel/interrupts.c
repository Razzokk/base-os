#include "interrupts.h"

#include <string.h>
#include "debug.h"
#include "misc.h"
#include "exceptions.h"

gate_descriptor idt[NUM_INTERRUPTS] __attribute__((aligned(IDT_ALIGNMENT)));

void set_idt_desc(size_t index, void* isr, uint8_t segment_selector, uint8_t ist, uint8_t type_attributes)
{
	gate_descriptor* entry = idt + index;

	entry->isr_low = (uint64_t)isr & UINT16_MAX;
	entry->segment_selector = segment_selector;
	entry->ist = ist & 0x7;
	entry->type_attributes = type_attributes;
	entry->isr_mid = ((uint64_t)isr >> 16) & UINT16_MAX;
	entry->isr_high = ((uint64_t)isr >> 32) & UINT32_MAX;
	entry->reserved = 0;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

void setup_interrupts()
{
	idt_ptr idtr;
	idtr.limit = sizeof(idt) - 1;
	idtr.base = idt;

	memset(idt, 0, sizeof(idt));

	setup_exceptions();

	asm volatile("lidt %[idtr]" :: [idtr]"m"(idtr));
}

#pragma GCC diagnostic pop
