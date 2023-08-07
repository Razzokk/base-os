#include "multiboot.h"

#include "kdefs.h"
#include "debug.h"
#include "memory.h"
#include "paging.h"
#include "string.h"

extern void kmain(void);

extern void* boot_stack_top;
extern void* kernel_start_;
extern void* kernel_end_;

void* kernel_start_address = &kernel_start_;
void* kernel_end_address = &kernel_end_;

extern pml4_entry boot_pml4_table[PML4_ENTRIES];
extern pml4_entry boot_pml3_table[PML4_ENTRIES];
extern pml4_entry boot_pml2_table[PML4_ENTRIES];

void handle_multiboot_mmap_tag(const multiboot_mmap_tag* tag)
{
	const multiboot_mmap_entry* entry = tag->entries;
	debugf("MMap entries (entry size: %d bytes):\n", tag->entry_size);
	size_t num_blocks = 0;
	size_t num_usable_blocks = 0;

	while ((uintptr_t) entry < ((uintptr_t) tag) + tag->tag.size)
	{
		debugf("- Base addr:	%p\n", entry->base_address);
		debugf("  Type:		%d\n", entry->type);
		debugf("  Length:	%d bytes\n", entry->length);
		++num_blocks;

		if (entry->type == MULTIBOOT_MEMORY_MAP_AVAILABLE)
			++num_usable_blocks;

		entry = (const multiboot_mmap_entry*) align_up(((uintptr_t) entry) + tag->entry_size, MULTIBOOT_TAG_ALIGN);
	}

	debugf("Summary: Number of blocks: %d, Number of usable blocks: %d\n", num_blocks, num_usable_blocks);
}

void handle_multiboot_tag(const multiboot_tag* tag)
{
	switch (tag->type)
	{
		case MULTIBOOT_TAG_TYPE_CMD_LINE:
			debugf("Command Line: %s\n", ((const multiboot_cmd_line_tag*) tag)->string);
			break;
		case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
			debugf("Boot loader name: %s\n", ((const multiboot_cmd_line_tag*) tag)->string);
			break;
		case MULTIBOOT_TAG_TYPE_MMAP:
			handle_multiboot_mmap_tag((const multiboot_mmap_tag*) tag);
			break;
	}
}

void parse_multiboot(const multiboot_info_struct* mboot_info)
{
	debug_literal(DBG_GREEN "Parsing Multiboot info structure...\n");
	debugf("Total Size: %d bytes\n", mboot_info->total_size);

	const multiboot_tag* tag = align_up((const multiboot_tag*) mboot_info->tags, MULTIBOOT_TAG_ALIGN);

	while (tag->type != MULTIBOOT_TAG_TYPE_END)
	{
		handle_multiboot_tag(tag);
		tag = (const multiboot_tag*) align_up((uintptr_t) tag + tag->size, MULTIBOOT_TAG_ALIGN);
	}

	debug_literal(DBG_GREEN "Parsing Multiboot info structure done.\n");
}

void entry(uint32_t mboot_magic, const multiboot_info_struct* mboot_info)
{
	mboot_info = (const multiboot_info_struct*) ((uintptr_t) mboot_info + KERNEL_VMA);

	if (mboot_magic == MULTIBOOT2_LOADER_MAGIC)
	{
		debugf(DBG_GREEN "Multiboot loader magic matches => %x\n", MULTIBOOT2_LOADER_MAGIC);
	} else
	{
		debug_literal(DBG_RED "Multiboot loader magic wrong!\n");
		debugf("Expected: %d, was: %d\n", MULTIBOOT2_LOADER_MAGIC, mboot_magic);
		asm volatile("hlt");
	}

	debugf(DBG_GREEN "Multiboot info struct address: %p\n", mboot_info);
	debugf(DBG_GREEN "Boot stack address:            %p\n", &boot_stack_top);
	debugf(DBG_GREEN "Kernel start address:          %p\n", kernel_start_address);
	debugf(DBG_GREEN "Kernel end address:            %p\n", kernel_end_address);

	debug_literal(DBG_GREEN "Unmap first pml4 entry and reload paging structure...\n");
	memset(boot_pml4_table, 0, sizeof(pml4_entry));

	parse_multiboot(mboot_info);
	kmain();
}
