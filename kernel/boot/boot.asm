bits 32

; Multiboot 2 header

MAGIC equ 0xE85250D6    ; multiboot 2 magic number
ARCH equ 0              ; 0 -> 32 bit protected mode
HDR_LEN equ (mboot_hdr_end - mboot_hdr_start)

section .multiboot_header
align 4
mboot_hdr_start:
    dd MAGIC
    dd ARCH
    dd HDR_LEN
    dd -(MAGIC + ARCH + HDR_LEN)
    
    ; multiboot end tag
    dw 0    ; type
    dw 0    ; flags
    dd 8    ; size
mboot_hdr_end:

; Kernel setup & start sequence

KERNEL_VMA equ 0xFFFFFF8000000000

global _start
extern _start64

section .text
_start:
    mov esp, (boot_stack_top - KERNEL_VMA)
    push eax	; save multiboot magic value
    push ebx	; save multiboot info structure pointer

    call setup_page_tables
    call enable_paging

    pop esi		; restore multiboot info structure pointer
    pop edi		; restore multiboot magic value

	[warning -number-overflow]
    lgdt [(gdt64.pointer - KERNEL_VMA)]
    [warning *number-overflow]
    jmp gdt64.code_segment:(_start64 - KERNEL_VMA)

    cli
end:
    hlt
    jmp end


; Map first and last pml4 entry -> first: 0x0, last: 0xFFFFFF8000000000
setup_page_tables:
    ; identity mapping
    mov eax, (boot_pml3_table - KERNEL_VMA)
    or eax, 0b11    ; present, writable
    mov [(boot_pml4_table - KERNEL_VMA)], eax
    mov [(boot_pml4_table - KERNEL_VMA) + 4088], eax	; last pml4 entry
    
    mov eax, (boot_pml2_table - KERNEL_VMA)
    or eax, 0b11    ; present, writable
    mov [(boot_pml3_table - KERNEL_VMA)], eax

    ; map pml2 entries to 2MiB pages
    mov ecx, 0  ; counter

.map_pml2:
    mov eax, 0x200000   ; 2MiB
    mul ecx
    or eax, 0b10000011  ; present, writable, huge page
    [warning -number-overflow]
    mov [(boot_pml2_table - KERNEL_VMA) + ecx * 8], eax
	[warning *number-overflow]

    inc ecx         ; inc counter
    cmp ecx, 512    ; check if all pages are mapped
    jne .map_pml2   ; if not continue mapping

    ret


LONG_MODE_MAGIC equ 0xC0000080

enable_paging:
    ; pass page table location to cpu
    mov eax, (boot_pml4_table - KERNEL_VMA)
    mov cr3, eax
    
    ; enable PAE (physical address extension)
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; enable long mode
    mov ecx, LONG_MODE_MAGIC
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ret


PAGE_SIZE equ 4096

global boot_pml4_table
global boot_pml3_table
global boot_pml2_table

global boot_stack_top
global boot_stack_bot

section .bss
align 4096
boot_pml4_table:
    resb PAGE_SIZE
boot_pml3_table:
    resb PAGE_SIZE
boot_pml2_table:
    resb PAGE_SIZE
boot_stack_bot:
    resb PAGE_SIZE * 4
boot_stack_top:


section .rodata
gdt64:
    dq 0    	; zero entry
    dq 0    	; zero entry
.code_segment: equ $ - gdt64
	dw 0xFFFF	; limit_low
	dw 0		; base_low
	db 0		; base_mid_low
	db 0x9A		; access_byte
	db 0xAF		; limit_high (lower 4 bits), flags (higher 4 bits)
	db 0		; base_mid_high
.data_segment: equ $ - .code_segment
	dw 0xFFFF	; limit_low
	dw 0		; base_low
	db 0		; base_mid_low
	db 0x92		; access_byte
	db 0xCF		; limit_high (lower 4 bits), flags (higher 4 bits)
	db 0		; base_mid_high
.pointer:
    dw $ - gdt64 - 1
    dq gdt64
