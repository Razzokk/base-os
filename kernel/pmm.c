#include "pmm.h"

#include <limits.h>
#include "kdefs.h"
#include "debug.h"
#include "paging.h"
#include "string.h"

extern size_t kernel_start_address;
extern size_t kernel_end_address;

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

static size_t lowest_usable_page;
static size_t highest_usable_page;

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

size_t pmm_alloc_page(void)
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

	return index + lowest_usable_page;
}

void pmm_free_page(size_t index)
{
	clear_bit(index - lowest_usable_page);
}

void* pmm_init(const multiboot_mmap_tag* mmap_info)
{
	void* mmap_info_end = (void*) offset_pointer(mmap_info, mmap_info->tag.size);
	void* lowest_usable_address = (void*) ((uintptr_t) -1);
	void* highest_usable_address = NULL;

	for (const multiboot_mmap_entry* entry = mmap_info->entries;
	(void*) entry < mmap_info_end;
	entry = (const multiboot_mmap_entry*) align_up(offset_pointer(entry, mmap_info->entry_size), MULTIBOOT_TAG_ALIGN))
	{
		if (entry->type != MULTIBOOT_MEMORY_MAP_AVAILABLE) continue;

		if (entry->base_address < lowest_usable_address)
			lowest_usable_address = entry->base_address;

		if (offset_pointer(entry->base_address, entry->length - 1) > highest_usable_address)
			highest_usable_address = offset_pointer(entry->base_address, entry->length - 1);
	}

	assert(lowest_usable_address != (void*) ((uintptr_t) -1));
	assert(highest_usable_address != NULL);

	// We need to align the lowest address to account for bytes which are not page aligned
	lowest_usable_page = addr2page(align_up(lowest_usable_address, PAGE_SIZE), PAGE_SIZE);
	// Here we would have to align down to page size but that is implicit in the conversion to page
	highest_usable_page = addr2page(highest_usable_address, PAGE_SIZE);

	size_t num_pages = highest_usable_page - lowest_usable_page + 1;
	bitmap = (value_type*) align_up(kernel_end_address, PAGE_SIZE);
	bitmap_size = num_pages;

	size_t size = align_up(bitmap_size, BITMAP_BITS) / CHAR_BIT;
	size_t num_free_pages = num_pages;
	memset(bitmap, 0, size);

	// Mark pages that are not of type available
	for (const multiboot_mmap_entry* entry = mmap_info->entries;
	(void*) entry < mmap_info_end;
	entry = (const multiboot_mmap_entry*) align_up(offset_pointer(entry, mmap_info->entry_size), MULTIBOOT_TAG_ALIGN))
	{
		if (entry->type == MULTIBOOT_MEMORY_MAP_AVAILABLE || entry->base_address < lowest_usable_address || entry->base_address > highest_usable_address)
			continue;

		uintptr_t start_page = addr2page(entry->base_address, PAGE_SIZE);
		uintptr_t end_page = addr2page(offset_pointer(entry->base_address, entry->length), PAGE_SIZE);

		for (size_t i = start_page; i < end_page; ++i)
		{
			set_bit(i);
			--num_free_pages;
		}
	}

	// Mark pages used by kernel and page manager
	void* end_address = align_up(offset_pointer(bitmap, size), PAGE_SIZE);
	size_t num_kernel_pages = ((uintptr_t) end_address - (uintptr_t) kernel_start_address) / PAGE_SIZE;

	for (size_t i = 0; i < num_kernel_pages; ++i)
	{
		set_bit(i);
		--num_free_pages;
	}

	debug_literal(DBG_GREEN "Initialize page manager:\n");
	debugf("Total number of pages: %d\n", num_pages);
	debugf("Number of free pages:  %d\n", num_free_pages);
	debugf("Bitmap address:        %p\n", bitmap);
	debugf("Bitmap size:           %d bits\n", bitmap_size);
	debugf("Size in bytes:         %d bytes\n", size);

	return end_address;
}
