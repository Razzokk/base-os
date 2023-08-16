#pragma once

#include <stddef.h>
#include <stdint.h>

void kbrk(void* address);

void* ksbrk(intptr_t increment);

void* kmalloc(size_t size, uint64_t alignment);

void kheap_init(void* heap_start, size_t capacity);
