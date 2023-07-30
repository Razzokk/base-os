#include "interrupts/irq.h"

#include <stdbool.h>
#include <sys/io.h>
#include "interrupts/pic.h"
#include "interrupts/interrupts.h"
#include "tbuf.h"
#include "terminal.h"
#include "debug.h"
#include "misc.h"

static char keycode_to_char[] =
{
	0, 0, '1', '2', '3', '4', '5' , '6',
	'7', '8', '9', '0', '-', '=', '\b', '\t',
	'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
	'o', 'p', '[', ']', '\n', 0, 'a', 's',
	'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
	'\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/',0, '*',
	0, ' ', 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, '7',
	'8', '9', '-', '4', '5', '6', '+', '1',
	'2', '3', '0', '.', 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, '\n', 0, '/'
};

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

__attribute__((interrupt)) void keyboard_interrupt_handler(const interrupt_frame* frame)
{
	uint8_t scancode = inb(0x60);
	extern terminal_t terminal;
	bool released = scancode & 0x80;
	uint8_t keycode = scancode & 0x7F;

	if (!released && keycode < sizeof(keycode_to_char))
	{
		char character = keycode_to_char[keycode];
		if (character != 0)
			term_putchar(&terminal, character);
	}

	pic_send_eoi(1);
}

#pragma GCC diagnostic pop
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

static inline void set_irq_idt(size_t irq_number, void* handler)
{
	set_idt_desc(NUM_EXCEPTIONS + irq_number, handler, 0x10, 0, GATE_TYPE_INTERRUPT);
}

void setup_irqs()
{
	set_irq_idt(0, timer_interrupt_handler);
	set_irq_idt(1, keyboard_interrupt_handler);
	set_irq_idt(2, irq_2_handler);
	set_irq_idt(3, irq_3_handler);
	set_irq_idt(4, irq_4_handler);
	set_irq_idt(5, irq_5_handler);
	set_irq_idt(6, irq_6_handler);
	set_irq_idt(7, irq_7_handler);
	set_irq_idt(8, irq_8_handler);
	set_irq_idt(9, irq_9_handler);
	set_irq_idt(10, irq_10_handler);
	set_irq_idt(11, irq_11_handler);
	set_irq_idt(12, irq_12_handler);
	set_irq_idt(13, irq_13_handler);
	set_irq_idt(14, irq_14_handler);
	set_irq_idt(15, irq_15_handler);

	pic_init(PIC_MASTER_OFFSET, PIC_SLAVE_OFFSET);
	pic_disable_all();

	for (uint8_t i = 0; i < 16; ++i)
		pic_send_eoi(i);

	pic_enable_irq(2);	// IRQ to cascade to slave pic
}

#pragma GCC diagnostic pop
