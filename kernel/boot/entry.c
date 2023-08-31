#include "multiboot.h"

#include "kdefs.h"
#include "debug.h"
#include "memory.h"
#include "paging.h"
#include "pmm.h"
#include "kheap.h"
#include "string.h"

extern void kmain(void);

extern void* boot_stack_top;
extern void* kernel_start_;
extern void* kernel_end_;

void* kernel_start_address = &kernel_start_;
void* kernel_end_address = &kernel_end_;

static const multiboot_mmap_tag* mmap_info = NULL;

void handle_multiboot_mmap_tag(const multiboot_mmap_tag* tag)
{
	debugf("MMap entries (entry size: %d bytes):\n", tag->entry_size);
	size_t num_blocks = 0;
	size_t num_usable_blocks = 0;
	void* tag_end = (void*) offset_pointer(tag, tag->tag.size);

	for (const multiboot_mmap_entry* entry = tag->entries;
	(void*) entry < tag_end;
	entry = (const multiboot_mmap_entry*) align_up(offset_pointer(entry, tag->entry_size), MULTIBOOT_TAG_ALIGN))
	{
		debugf("- Base addr: %p\n", entry->base_address);
		debugf("  Type:      %d\n", entry->type);
		debugf("  Length:    %d bytes\n", entry->length);
		++num_blocks;

		if (entry->type == MULTIBOOT_MEMORY_MAP_AVAILABLE)
			++num_usable_blocks;
	}

	debug_literal("Summary:\n");
	debugf("- Number of blocks:        %d\n", num_blocks);
	debugf("- Number of usable blocks: %d\n", num_usable_blocks);
}

void handle_multiboot_tag(const multiboot_tag* tag)
{
	switch (tag->type)
	{
		case MULTIBOOT_TAG_TYPE_CMD_LINE:
			debugf("Command Line: %s\n", ((const multiboot_cmd_line_tag*) tag)->string);
			break;
		case MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME:
			debugf("Boot loader name: %s\n", ((const multiboot_load_name_tag*) tag)->string);
			break;
		case MULTIBOOT_TAG_TYPE_MMAP:
			mmap_info = (const multiboot_mmap_tag*) tag;
			handle_multiboot_mmap_tag(mmap_info);
			break;
	}
}

void parse_multiboot(const multiboot_info_struct* mboot_info)
{
	debug_literal(DBG_GREEN "Parsing Multiboot info structure...\n");
	debugf("Total Size: %d bytes\n", mboot_info->total_size);

	for (const multiboot_tag* tag = align_up((const multiboot_tag*) mboot_info->tags, MULTIBOOT_TAG_ALIGN);
	tag->type != MULTIBOOT_TAG_TYPE_END;
	tag = (const multiboot_tag*) align_up(offset_pointer(tag, tag->size), MULTIBOOT_TAG_ALIGN))
		handle_multiboot_tag(tag);

	debug_literal(DBG_GREEN "Parsing Multiboot info structure done.\n");
}

void entry(uint32_t mboot_magic, const multiboot_info_struct* mboot_info)
{
	mboot_info = (const multiboot_info_struct*) P2V(mboot_info);

	if (mboot_magic == MULTIBOOT2_LOADER_MAGIC)
	{
		debugf(DBG_GREEN "Multiboot loader magic matches => %x\n", MULTIBOOT2_LOADER_MAGIC);
	}
	else
	{
		debug_literal(DBG_RED "Multiboot loader magic wrong!\n");
		debugf("Expected: %d, was: %d\n", MULTIBOOT2_LOADER_MAGIC, mboot_magic);
		asm volatile("hlt");
	}

	debugf(DBG_GREEN "Multiboot info struct address: %p\n", mboot_info);
	debugf(DBG_GREEN "Boot stack address:            %p\n", &boot_stack_top);
	debugf(DBG_GREEN "Kernel start address:          %p\n", kernel_start_address);
	debugf(DBG_GREEN "Kernel end address:            %p\n", kernel_end_address);

	debug_literal("Copy multiboot info struct to end of kernel...\n");

	memcpy(kernel_end_address, mboot_info, mboot_info->total_size);
	mboot_info = kernel_end_address;
	kernel_end_address = offset_pointer(kernel_end_address, mboot_info->total_size);

	debugf(DBG_GREEN "New multiboot info struct address: %p\n", mboot_info);
	debugf(DBG_GREEN "New kernel end address:            %p\n", kernel_end_address);
	debug_literal("Copy multiboot info struct to end of kernel done.\n");

	parse_multiboot(mboot_info);
	void* end_ = pmm_init(mmap_info);

	void* heap = align_up(end_, PAGE_SIZE);
	kheap_init(heap, 2 * MiB);

	debug_literal(DBG_GREEN "Unmap boot paging structure and reload...\n");

	pml4_table_t* pml4_table = (pml4_table_t*) kmalloc(sizeof(pml4_table_t), PAGE_SIZE);
	pml3_table_t* pml3_table = (pml3_table_t*) kmalloc(sizeof(pml3_table_t), PAGE_SIZE);
	pml2_table_t* pml2_table = (pml2_table_t*) kmalloc(sizeof(pml2_table_t), PAGE_SIZE);
	pml1_table_t* pml1_table = (pml1_table_t*) kmalloc(sizeof(pml1_table_t), PAGE_SIZE);
	memset(pml4_table, 0, sizeof(pml4_table_t));
	memset(pml3_table, 0, sizeof(pml3_table_t));
	memset(pml2_table, 0, sizeof(pml2_table_t));
	memset(pml1_table, 0, sizeof(pml1_table_t));

	for (size_t i = 0; i < NUM_PML1_ENTRIES; ++i)
	{
		pml1_entry* entry = *pml1_table + i;
		entry->present = 1;
		entry->read_write = 1;
		entry->ppn = i;
	}

	pml2_entry* pml2_entry = *pml2_table;
	pml2_entry->present = 1;
	pml2_entry->read_write = 1;
	pml2_entry->ppn = V2P(pml1_table) / PAGE_SIZE;

	pml3_entry* pml3_entry = *pml3_table;
	pml3_entry->present = 1;
	pml3_entry->read_write = 1;
	pml3_entry->ppn = V2P(pml2_table) / PAGE_SIZE;

	pml4_entry* pml4_entry = *pml4_table + NUM_PML4_ENTRIES - 1;
	pml4_entry->present = 1;
	pml4_entry->read_write = 1;
	pml4_entry->ppn = V2P(pml3_table) / PAGE_SIZE;

	set_pml4(pml4_table);

	debug_literal(DBG_GREEN "Unmap boot paging structure and reload done.\n");

	kmain();
}
