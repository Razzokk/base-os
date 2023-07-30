#include "interrupts/pic.h"

void pic_init(uint8_t offset_master, uint8_t offset_slave)
{
	// Store pic masks
	uint8_t master_mask = inb(PIC_PORT_MASTER_DATA);
	uint8_t slave_mask = inb(PIC_PORT_SLAVE_DATA);

	// starts the initialization sequence (in cascade mode)
	outb_p(PIC_PORT_MASTER_CMD, ICW1_INIT | ICW1_ICW4);
	outb_p(PIC_PORT_SLAVE_CMD, ICW1_INIT | ICW1_ICW4);

	// ICW2: PIC vector offsets
	outb_p(PIC_PORT_MASTER_DATA, offset_master);
	outb_p(PIC_PORT_SLAVE_DATA, offset_slave);

	outb_p(PIC_PORT_MASTER_DATA, 4);	// ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outb_p(PIC_PORT_SLAVE_DATA, 2);	// ICW3: tell Slave PIC its cascade identity (0000 0010)

	outb_p(PIC_PORT_MASTER_DATA, ICW4_8086);
	outb_p(PIC_PORT_SLAVE_DATA, ICW4_8086);

	// Restore saved pic masks
	outb_p(PIC_PORT_MASTER_DATA, master_mask);
	outb_p(PIC_PORT_SLAVE_DATA, slave_mask);
}

void pic_disable_all()
{
	outb(PIC_PORT_MASTER_DATA, UINT8_MAX);
	outb(PIC_PORT_SLAVE_DATA, UINT8_MAX);
}

void pic_send_eoi(uint8_t irq)
{
	if(irq >= 8)
		outb(PIC_PORT_SLAVE_CMD, PIC_EOI);
	outb(PIC_PORT_MASTER_CMD, PIC_EOI);
}

void pic_enable_irq(uint8_t irq)
{
	uint16_t port;

	if(irq < 8)
	{
		port = PIC_PORT_MASTER_DATA;
	}
	else
	{
		port = PIC_PORT_SLAVE_DATA;
		irq -= 8;
	}

	uint8_t value = inb(port) & ~(1 << irq);
	outb(port, value);
}

void pic_disable_irq(uint8_t irq)
{
	uint16_t port;

	if(irq < 8)
	{
		port = PIC_PORT_MASTER_DATA;
	}
	else
	{
		port = PIC_PORT_SLAVE_DATA;
		irq -= 8;
	}

	uint8_t value = inb(port) | (1 << irq);
	outb(port, value);
}
