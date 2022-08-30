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

global _start
extern _start64

section .text
_start:
    mov esp, stack_top

    call setup_page_tables
    call enable_paging

    lgdt [gdt64.pointer]
    jmp gdt64.code_segment:_start64

    cli
end:
    hlt
    jmp end


setup_page_tables:
    ; identity mapping
    mov eax, pml3_table
    or eax, 0b11    ; present, writable
    mov [pml4_table], eax
    
    mov eax, pml2_table
    or eax, 0b11    ; present, writable
    mov [pml3_table], eax

    ; map pml2 entries to 2MiB pages
    mov ecx, 0  ; counter

.map_pml2:
    mov eax, 0x200000   ; 2MiB
    mul ecx
    or eax, 0b10000011  ; present, writable, huge page
    mov [pml2_table + ecx * 8], eax

    inc ecx         ; inc counter
    cmp ecx, 512    ; check if all pages are mapped
    jne .map_pml2   ; if not continue mapping

    ret


LONG_MODE_MAGIC equ 0xC0000080

enable_paging:
    ; pass page table location to cpu
    mov eax, pml4_table
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

section .bss
align 4096
pml4_table:
    resb PAGE_SIZE
pml3_table:
    resb PAGE_SIZE
pml2_table:
    resb PAGE_SIZE
stack_bot:
    resb PAGE_SIZE * 4
stack_top:


section .rodata
gdt64:
    dq 0    ; zero entry
.code_segment: equ $ - gdt64
    dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)    ; code segment
.pointer:
    dw $ - gdt64 - 1
    dq gdt64
