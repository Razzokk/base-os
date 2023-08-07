global _start64
extern entry

extern boot_stack_top

section .text
bits 64
_start64:
	; don't change content's of rdi and rsi,
	; they contain the multiboot magic value and multiboot info structure pointer
	; they are passed as arguments to the entry function

    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov rsp, boot_stack_top

    call entry

    hlt
