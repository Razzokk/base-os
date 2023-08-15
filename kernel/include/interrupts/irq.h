#pragma once

#include "kdefs.h"
#include "interrupts.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline void set_irq_idt(size_t irq_number, fptr_t handler)
{
	set_idt_desc(NUM_EXCEPTIONS + irq_number, handler, 0x10, 0, GATE_TYPE_INTERRUPT);
}

void setup_irqs(void);

#ifdef __cplusplus
}
#endif
