global _start64
extern kmain

section .text
bits 64
_start64:
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call kmain

    hlt
