#pragma once

#include <stddef.h>

size_t alloc_page(void);

void free_page(size_t);

void* page_manager_init(size_t num_pages, size_t first_usable_page);
