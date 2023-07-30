#include "exceptions.h"

#include "debug.h"
#include "misc.h"

#define default_handler(exception_number) \
__attribute__((interrupt)) void exception_handler_ ## exception_number(const interrupt_frame* frame) \
{ \
	default_exception_handler(frame, exception_number); \
}

#define default_handler_error_code(exception_number) \
__attribute__((interrupt)) void exception_handler_error_code_ ## exception_number(const interrupt_frame* frame, uint64_t error_code) \
{ \
	default_exception_handler_error_code(frame, error_code, exception_number); \
}

const char* const EXCEPTION_NAMES[NUM_EXCEPTIONS] =
{
	"#DE: Divide-by-zero Error",
	"#DB: Debug",
	"Non-maskable Interrupt",
	"#BP: Breakpoint",
	"#OF: Overflow",
	"#BR: Bound Range Exceeded",
	"#UD: Invalid Opcode",
	"#NM: Device Not Available",
	"#DF: Double Fault",
	"Coprocessor Segment Overrun",
	"#TS: Invalid TSS",
	"#NP: Segment Not Present",
	"#SS: Stack-Segment Fault",
	"#GP: General Protection Fault",
	"#PF: Page Fault",
	"RESERVED - What is going on?!?",
	"#MF: x87 Floating-Point Exception",
	"#AC: Alignment Check",
	"#MC: Machine Check",
	"#XM: SIMD Floating-Point Exception",
	"#VE: Virtualization Exception",
	"#CP: Control Protection Exception",
	"RESERVED - What is going on?!?",
	"RESERVED - What is going on?!?",
	"RESERVED - What is going on?!?",
	"RESERVED - What is going on?!?",
	"RESERVED - What is going on?!?",
	"RESERVED - What is going on?!?",
	"#HV: Hypervisor Injection Exception",
	"#VC: VMM Communication Exception",
	"#SX: Security Exception",
};

void print_frame(const interrupt_frame* frame)
{
	char buffer[ULLTOA_BUF_SIZE];

	debug_literal("- rip:\t\t");
	ulltoa(frame->rip, buffer, 16);
	debug(buffer, ULLTOA_BUF_SIZE);

	debug_literal("\n- cs:\t\t");
	ulltoa(frame->cs, buffer, 16);
	debug(buffer, ULLTOA_BUF_SIZE);

	debug_literal("\n- rflags:\t");
	ulltoa(frame->rflags, buffer, 16);
	debug(buffer, ULLTOA_BUF_SIZE);

	debug_literal("\n- rsp:\t\t");
	ulltoa(frame->rsp, buffer, 16);
	debug(buffer, ULLTOA_BUF_SIZE);

	debug_literal("\n- ss:\t\t");
	ulltoa(frame->ss, buffer, 16);
	debug(buffer, ULLTOA_BUF_SIZE);
	debug_char('\n');
}

void default_exception_handler(const interrupt_frame* frame, size_t exception_number)
{
	debug_literal("[\x1b[91mDEBUG\x1b[m]: ");
	debug(EXCEPTION_NAMES[exception_number], 48);
	debug_literal(" occurred!\n");
	print_frame(frame);
	disable_interrupts();
	asm volatile("hlt");
}

void default_exception_handler_error_code(const interrupt_frame* frame, uint64_t error_code, size_t exception_number)
{
	debug_literal("[\x1b[91mDEBUG\x1b[m]: ");
	debug(EXCEPTION_NAMES[exception_number], 48);
	debug_literal(" occurred!\n");

	char buffer[ULLTOA_BUF_SIZE];
	debug_literal("- err_code:\t");
	ulltoa(error_code, buffer, 16);
	debug(buffer, ULLTOA_BUF_SIZE);
	debug_char('\n');

	print_frame(frame);
	disable_interrupts();
	asm volatile("hlt");
}

default_handler(0)
default_handler(1)
default_handler(2)
default_handler(3)
default_handler(4)
default_handler(5)
default_handler(6)
default_handler(7)
default_handler_error_code(8)
default_handler(9)
default_handler_error_code(10)
default_handler_error_code(11)
default_handler_error_code(12)
default_handler_error_code(13)
default_handler(15)
default_handler(16)
default_handler_error_code(17)
default_handler(18)
default_handler(19)
default_handler(20)
default_handler_error_code(21)
default_handler(22)
default_handler(23)
default_handler(24)
default_handler(25)
default_handler(26)
default_handler(27)
default_handler(28)
default_handler_error_code(29)
default_handler_error_code(30)
default_handler(31)

__attribute__((interrupt)) void pagefault_handler(const interrupt_frame* frame, uint64_t error_code)
{
	debug_literal("[\x1b[91mDEBUG\x1b[m]: Page Fault occurred!\n");

	char buffer[ULLTOA_BUF_SIZE];
	debug_literal("- address:\t");
	uint64_t address;
	asm volatile("mov %%cr2, %%rax" : "=a"(address));
	ulltoa(address, buffer, 16);
	debug(buffer, ULLTOA_BUF_SIZE);
	debug_char('\n');

	debug_literal("- err_code:\t");
	ulltoa(error_code, buffer, 16);
	debug(buffer, ULLTOA_BUF_SIZE);
	debug_char('\n');

	print_frame(frame);
	disable_interrupts();
	asm volatile("hlt");
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

void setup_exceptions()
{
	set_idt_desc(0, (void*)exception_handler_0, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(1, (void*)exception_handler_1, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(2, (void*)exception_handler_2, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(3, (void*)exception_handler_3, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(4, (void*)exception_handler_4, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(5, (void*)exception_handler_5, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(6, (void*)exception_handler_6, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(7, (void*)exception_handler_7, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(8, (void*)exception_handler_error_code_8, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(9, (void*)exception_handler_9, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(10, (void*)exception_handler_error_code_10, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(11, (void*)exception_handler_error_code_11, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(12, (void*)exception_handler_error_code_12, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(13, (void*)exception_handler_error_code_13, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(14, (void*)pagefault_handler, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(15, (void*)exception_handler_15, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(16, (void*)exception_handler_16, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(17, (void*)exception_handler_error_code_17, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(18, (void*)exception_handler_18, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(19, (void*)exception_handler_19, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(20, (void*)exception_handler_20, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(21, (void*)exception_handler_error_code_21, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(22, (void*)exception_handler_22, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(23, (void*)exception_handler_23, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(24, (void*)exception_handler_24, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(25, (void*)exception_handler_25, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(26, (void*)exception_handler_26, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(27, (void*)exception_handler_27, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(28, (void*)exception_handler_28, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(29, (void*)exception_handler_error_code_29, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(30, (void*)exception_handler_error_code_30, 0x10, 0, GATE_TYPE_TRAP);
	set_idt_desc(31, (void*)exception_handler_31, 0x10, 0, GATE_TYPE_TRAP);
}

#pragma GCC diagnostic pop
