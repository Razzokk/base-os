#pragma once

#include <stdint.h>
#include <sys/io.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PIC_MASTER_OFFSET	32
#define PIC_SLAVE_OFFSET	40
#define PIC_SLAVE_NUM_IRQS	8
#define PIC_SLAVE_NUM_IRQS	8

#define PIC_PORT_MASTER_CMD		0x20
#define PIC_PORT_MASTER_DATA	0x21
#define PIC_PORT_SLAVE_CMD		0xA0
#define PIC_PORT_SLAVE_DATA		0xA1

#define PIC_EOI	0x20 // End-of-interrupt command code

// Initialize words
#define ICW1_ICW4		0x01 // ICW4 (not) needed
#define ICW1_SINGLE		0x02 // Single (cascade) mode
#define ICW1_INTERVAL4	0x04 // Call address interval 4 (8)
#define ICW1_LEVEL		0x08 // Level triggered (edge) mode
#define ICW1_INIT		0x10 // Initialization - required!

#define ICW4_8086		0x01 // 8086/88 (MCS-80/85) mode
#define ICW4_AUTO		0x02 // Auto (normal) EOI
#define ICW4_BUF_SLAVE	0x08 // Buffered mode/slave
#define ICW4_BUF_MASTER	0x0C // Buffered mode/master
#define ICW4_SFNM		0x10 // Special fully nested (not)

/**
 * Initialize both (master and slave) Programmable Interrupt Controllers
 *
 * @param offset_master the idt offset for the master pic
 * @param offset_slave the idt offset for the slave pic
 */
void pic_init(uint8_t offset_master, uint8_t offset_slave);

void pic_disable_all();

void pic_disable_irq(uint8_t irq);

void pic_enable_irq(uint8_t irq);

void pic_send_eoi(uint8_t irq);

#ifdef __cplusplus
}
#endif
