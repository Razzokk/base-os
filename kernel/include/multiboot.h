#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// The multiboot 2 specification contains a lot more features than provided in here, but we want to keep it simple

#define MULTIBOOT2_MAGIC		0xE85250D6
#define MULTIBOOT2_LOADER_MAGIC	0x36D76289

// Alignments

#define MULTIBOOT_INFO_ALIGN	8
#define MULTIBOOT_TAG_ALIGN		8

// Tag Types

#define MULTIBOOT_TAG_TYPE_END				0
#define MULTIBOOT_TAG_TYPE_CMD_LINE			1
#define MULTIBOOT_TAG_TYPE_BOOT_LOADER_NAME	2
#define MULTIBOOT_TAG_TYPE_MMAP				6

// Memory Map

#define MULTIBOOT_MEMORY_MAP_AVAILABLE			1
#define MULTIBOOT_MEMORY_MAP_RESERVED			2
#define MULTIBOOT_MEMORY_MAP_ACPI_RECLAIMABLE	3
#define MULTIBOOT_MEMORY_MAP_NVS				4
#define MULTIBOOT_MEMORY_MAP_BADRAM				5

typedef struct multiboot_tag
{
	uint32_t type;
	uint32_t size;
} __attribute__((packed)) multiboot_tag;

typedef struct multiboot_info
{
	uint32_t total_size;
	uint32_t reserved;
	multiboot_tag tags[];
} __attribute__((packed)) multiboot_info_struct;

typedef struct multiboot_string_tag
{
	multiboot_tag tag;
	char string[];
} __attribute__((packed)) multiboot_string_tag;

typedef multiboot_string_tag multiboot_cmd_line_tag;
typedef multiboot_string_tag multiboot_load_name_tag;

typedef struct multiboot_memory_map_entry
{
	void* base_address;
	uint64_t length;
	uint32_t type;
	uint32_t reserved;
} __attribute__((packed)) multiboot_mmap_entry;

typedef struct multiboot_memory_map_tag
{
	multiboot_tag tag;
	uint32_t entry_size;
	uint32_t entry_version;
	multiboot_mmap_entry entries[];
} __attribute__((packed)) multiboot_mmap_tag;

#ifdef __cplusplus
}
#endif
