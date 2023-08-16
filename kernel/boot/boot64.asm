global _start64
extern entry

extern boot_stack_top
extern gdt64.pointer

section .text
bits 64
_start64:
	; Don't change content's of rdi and rsi,
	; they contain the multiboot magic value and multiboot info structure pointer
	; they are passed as arguments to the entry function

	; reload gdt
	mov rax, gdt64.pointer
	lgdt [rax]

    mov ax, 0x20
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

	; Perform absolute jump
    mov rax, _start64.jmp
    jmp rax

_start64.jmp:
	; Setup stack
    mov rsp, boot_stack_top
    mov rbp, rsp

    call entry

    hlt
