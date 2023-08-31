#pragma once

#include <stddef.h>
#include "multiboot.h"

size_t pmm_alloc_page(void);

void pmm_free_page(size_t index);

void* pmm_init(const multiboot_mmap_tag* mmap_info);
