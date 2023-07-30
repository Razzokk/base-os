#include "interrupts/irq.h"

#include "interrupts/pic.h"
#include "interrupts/interrupts.h"
#include "tbuf.h"
#include "terminal.h"

#define default_handler(irq_number) \
__attribute__((interrupt)) void irq_ ## irq_number ## _handler(const interrupt_frame* frame) \
{ \
	default_irq_handler(frame, irq_number); \
}

#define TIMER_INTERRUPT_FREQUENCY_NS 54925439ULL

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void default_irq_handler(const interrupt_frame* frame, uint8_t irq_number)
{
	pic_send_eoi(irq_number);
}

default_handler(1)
default_handler(2)
default_handler(3)
default_handler(4)
default_handler(5)
default_handler(6)
default_handler(7)
default_handler(8)
default_handler(9)
default_handler(10)
default_handler(11)
default_handler(12)
default_handler(13)
default_handler(14)
default_handler(15)

__attribute__((interrupt)) void timer_interrupt_handler(const interrupt_frame* frame)
{
	static size_t counter = 0;
	const uint8_t display[] = {180, 217, 193, 192, 195, 218, 194, 191};
	tbuf_write(0, TEXT_BUF_COLS - 1, (char)display[counter % sizeof(display)], BROWN, LIGHT_BLUE);
	++counter;
	pic_send_eoi(0);
}

#pragma GCC diagnostic pop
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

void setup_irqs()
{
	set_idt_desc(NUM_EXCEPTIONS + 0, timer_interrupt_handler, 0x10, 0, GATE_TYPE_INTERRUPT);
	set_idt_desc(NUM_EXCEPTIONS + 1, irq_1_handler, 0x10, 0, GATE_TYPE_INTERRUPT);
	set_idt_desc(NUM_EXCEPTIONS + 2, irq_2_handler, 0x10, 0, GATE_TYPE_INTERRUPT);
	set_idt_desc(NUM_EXCEPTIONS + 3, irq_3_handler, 0x10, 0, GATE_TYPE_INTERRUPT);
	set_idt_desc(NUM_EXCEPTIONS + 4, irq_4_handler, 0x10, 0, GATE_TYPE_INTERRUPT);
	set_idt_desc(NUM_EXCEPTIONS + 5, irq_5_handler, 0x10, 0, GATE_TYPE_INTERRUPT);
	set_idt_desc(NUM_EXCEPTIONS + 6, irq_6_handler, 0x10, 0, GATE_TYPE_INTERRUPT);
	set_idt_desc(NUM_EXCEPTIONS + 7, irq_7_handler, 0x10, 0, GATE_TYPE_INTERRUPT);
	set_idt_desc(NUM_EXCEPTIONS + 8, irq_8_handler, 0x10, 0, GATE_TYPE_INTERRUPT);
	set_idt_desc(NUM_EXCEPTIONS + 9, irq_9_handler, 0x10, 0, GATE_TYPE_INTERRUPT);
	set_idt_desc(NUM_EXCEPTIONS + 10, irq_10_handler, 0x10, 0, GATE_TYPE_INTERRUPT);
	set_idt_desc(NUM_EXCEPTIONS + 11, irq_11_handler, 0x10, 0, GATE_TYPE_INTERRUPT);
	set_idt_desc(NUM_EXCEPTIONS + 12, irq_12_handler, 0x10, 0, GATE_TYPE_INTERRUPT);
	set_idt_desc(NUM_EXCEPTIONS + 13, irq_13_handler, 0x10, 0, GATE_TYPE_INTERRUPT);
	set_idt_desc(NUM_EXCEPTIONS + 14, irq_14_handler, 0x10, 0, GATE_TYPE_INTERRUPT);
	set_idt_desc(NUM_EXCEPTIONS + 15, irq_15_handler, 0x10, 0, GATE_TYPE_INTERRUPT);

	pic_init(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
	pic_disable_all();

	for (uint8_t i = 0; i < 16; ++i)
		pic_send_eoi(i);

	pic_enable_irq(2);	// IRQ to cascade to slave pic
}

#pragma GCC diagnostic pop
