# Compiler flags
CFLAGS := -std=gnu99 -ffreestanding -O0 -Wall -Wextra
LINKFLAGS := -nostdlib -lgcc -z max-page-size=4096 $(CFLAGS)

# Compilers
CC := ./cross/bin/x86_64-elf-gcc

# Files
ISO_FILENAME := "base_os"

KERNEL_ASM_SRC_FILES := $(shell find kernel/ -name *.asm)
KERNEL_C_SRC_FILES := $(shell find kernel/ -name *.c)

KERNEL_ASM_OBJ_FILES := $(patsubst kernel/%.asm, build/kernel/%.o, $(KERNEL_ASM_SRC_FILES))
KERNEL_C_OBJ_FILES := $(patsubst kernel/%.c, build/kernel/%.o, $(KERNEL_C_SRC_FILES))

$(KERNEL_ASM_OBJ_FILES): build/%.o : %.asm
	mkdir -p $(dir $@) && \
	nasm -o $@ $< -f elf64

$(KERNEL_C_OBJ_FILES): build/%.o : %.c
	mkdir -p $(dir $@) && \
	$(CC) -o $@ -c $< -I kernel/include -I libc/include $(CFLAGS)

# Targets
.PHONY: all clean build
.SUFFIXES: .asm .c .o .ld

all: clean build

clean:
	rm -rf build/*

build: $(KERNEL_ASM_OBJ_FILES) $(KERNEL_C_OBJ_FILES) cfg/linker.ld
	mkdir -p build/boot/grub && \
	$(CC) -T cfg/linker.ld -o build/boot/kernel.bin $(LINKFLAGS) $(KERNEL_ASM_OBJ_FILES) $(KERNEL_C_OBJ_FILES) && \
	cp cfg/grub.cfg build/boot/grub/ && \
	rm -f build/$(ISO_FILENAME).iso
	grub-mkrescue /usr/lib/grub/i386-pc -o build/$(ISO_FILENAME).iso build
