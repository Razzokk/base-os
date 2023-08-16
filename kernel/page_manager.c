#include "page_manager.h"

#include <limits.h>
#include "kdefs.h"
#include "debug.h"
#include "paging.h"
#include "string.h"

// ===========================================================
// bitmap
// ===========================================================

typedef uint64_t value_type;
#define BITMAP_BITS (CHAR_BIT * sizeof(value_type))
#define BITMAP_INVALID_INDEX (-1ULL)

#define entry_index(bit) (bit / BITMAP_BITS)
#define bit_index(bit) (bit % BITMAP_BITS)

static value_type* bitmap = NULL;
static size_t bitmap_size = 0;
static size_t start_page = 0;

static void set_bit(size_t bit)
{
	size_t index = entry_index(bit);
	size_t offset = bit_index(bit);
	bitmap[index] |= 1ULL << offset;
}

static void clear_bit(size_t bit)
{
	size_t index = entry_index(bit);
	size_t offset = bit_index(bit);
	bitmap[index] &= ~(1ULL << offset);
}

// ===========================================================
// page management
// ===========================================================

static size_t find_first_free_page(void)
{
	for (size_t index = 0; index < entry_index(bitmap_size); ++index)
	{
		if (bitmap[index] == -1ULL) continue;
		for (size_t offset = 0; offset < BITMAP_BITS; ++offset)
		{
			value_type test = 1ULL << offset;
			if ((bitmap[index] & test) == 0)
				return index * BITMAP_BITS + offset;
		}
	}
	return BITMAP_INVALID_INDEX;
}

size_t alloc_page(void)
{
	size_t index = find_first_free_page();

	if (index == BITMAP_INVALID_INDEX)
	{
		// We should probably panic here
		debugf(DBG_RED "No more free pages to allocate!\n", 0);
		return BITMAP_INVALID_INDEX;
	}
	else
	{
		set_bit(index);
	}

	return start_page + index;
}

void free_page(size_t index)
{
	clear_bit(index - start_page);
}

void* page_manager_init(size_t num_pages, size_t first_usable_page)
{
	extern size_t kernel_start_address;
	extern size_t kernel_end_address;

	bitmap = (value_type*) align_up(kernel_end_address, PAGE_SIZE);
	bitmap_size = num_pages;
	start_page = first_usable_page;

	size_t size = align_up(bitmap_size, BITMAP_BITS) / CHAR_BIT;
	memset(bitmap, 0, size);

	// Mark pages used by kernel and page manager
	void* end_address = align_up(offset_pointer(bitmap, size), PAGE_SIZE);
	size_t num_kernel_pages = ((uintptr_t) end_address - (uintptr_t) kernel_start_address) / PAGE_SIZE;

	for (size_t i = 0; i < num_kernel_pages; ++i)
		set_bit(i);

	debug_literal(DBG_GREEN "Initialize page manager:\n");
	debugf("Bitmap address: %p\n", bitmap);
	debugf("Bitmap size: %d bits\n", bitmap_size);
	debugf("Size in bytes: %d bytes\n", size);

	return end_address;
}
