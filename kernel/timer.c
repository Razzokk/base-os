#include "timer.h"
#include "interrupts/interrupts.h"
#include "interrupts/pic.h"
#include "tbuf.h"
#include "terminal.h"
#include "interrupts/irq.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

__attribute__((interrupt)) static void timer_handler(const interrupt_frame* frame)
{
	static size_t counter = 0;
	const uint8_t display[] = {180, 217, 193, 192, 195, 218, 194, 191};
	tbuf_write(0, TEXT_BUF_COLS - 1, (char) display[counter % sizeof(display)], BROWN, LIGHT_BLUE);
	++counter;
	pic_send_eoi(0);
}

#pragma GCC diagnostic pop

void timer_init(uint32_t frequency_hz)
{
	set_irq_idt(0, (fptr_t) timer_handler);
	uint32_t divisor = 1193180 / frequency_hz;

	outb(0x43, 0x36);

	uint8_t low = (uint8_t) (divisor & 0xFF);
	uint8_t high = (uint8_t) ((divisor >> 8) & 0xFF);

	outb(0x40, low);
	outb(0x40, high);

	pic_enable_irq(0);
}
