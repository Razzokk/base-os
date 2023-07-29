# General
OS_VERSION := 0.1.0
TARGET := x86_64

# Compiler flags
CFLAGS := -std=gnu17 -Wall -Wextra -Wpedantic -ffreestanding -mno-red-zone -mno-mmx -mno-sse -mno-sse2
CFLAGS += -D OS_VERSION=$(OS_VERSION)
CFLAGS += -ggdb -O0
LINK_FLAGS	:= -nostdlib -lgcc -z max-page-size=4096

# Compilers
CC	:= $(TARGET)-elf-gcc

# Files
ISO_FILENAME := "base_os"

KERNEL_ASM_SRC_FILES := $(shell find kernel/ -name *.asm)
KERNEL_C_SRC_FILES := $(shell find kernel/ -name *.c)

KERNEL_ASM_OBJ_FILES := $(patsubst kernel/%.asm, build/kernel/%.o, $(KERNEL_ASM_SRC_FILES))
KERNEL_C_OBJ_FILES := $(patsubst kernel/%.c, build/kernel/%.o, $(KERNEL_C_SRC_FILES))

# Targets
.PHONY: all clean build
.SUFFIXES: .asm .c .o .ld

default: all

all: clean build

clean:
	rm -rf build/*

build: $(KERNEL_ASM_OBJ_FILES) $(KERNEL_C_OBJ_FILES) cfg/linker.ld
	mkdir -p build/boot/grub && \
	rm -f build/$(ISO_FILENAME).iso
	$(CC) -T cfg/linker.ld -o build/boot/kernel.bin $(LINK_FLAGS) $(KERNEL_ASM_OBJ_FILES) $(KERNEL_C_OBJ_FILES) && \
	cp cfg/grub.cfg build/boot/grub/ && \
	grub-mkrescue /usr/lib/grub/i386-pc -o build/$(ISO_FILENAME).iso build

$(KERNEL_ASM_OBJ_FILES): build/%.o : %.asm
	mkdir -p $(dir $@) && \
	nasm -o $@ $< -f elf64

$(KERNEL_C_OBJ_FILES): build/%.o : %.c
	mkdir -p $(dir $@) && \
	$(CC) -o $@ -c $< -I kernel/include -I libc/include $(CFLAGS)
