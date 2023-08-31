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
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER		8

// Memory Map

#define MULTIBOOT_MEMORY_MAP_AVAILABLE			1
#define MULTIBOOT_MEMORY_MAP_RESERVED			2
#define MULTIBOOT_MEMORY_MAP_ACPI_RECLAIMABLE	3
#define MULTIBOOT_MEMORY_MAP_NVS				4
#define MULTIBOOT_MEMORY_MAP_BADRAM				5

// Framebuffer types

#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED	0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB		1
#define MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT 2

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

typedef struct multiboot_tag
{
	uint32_t type;
	uint32_t size;
} __attribute__((packed)) multiboot_tag;

typedef struct multiboot_info_struct
{
	uint32_t total_size;
	uint32_t reserved;
	multiboot_tag tags[];
} __attribute__((packed)) multiboot_info_struct;

typedef struct multiboot_framebuffer_rgb
{
	uint8_t red_field_position;
	uint8_t red_mask_size;
	uint8_t green_field_position;
	uint8_t green_mask_size;
	uint8_t blue_field_position;
	uint8_t blue_mask_size;
} multiboot_framebuffer_rgb;

typedef struct multiboot_color
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
} multiboot_color;

typedef struct multiboot_framebuffer_indexed
{
	uint16_t palette_num_colors;
	multiboot_color palette[];
} multiboot_framebuffer_indexed;

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

typedef struct multiboot_tag_framebuffer
{
	multiboot_tag tag;
	uint64_t addr;
	uint32_t pitch;
	uint32_t width;
	uint32_t height;
	uint8_t bpp;
	uint8_t type;
	uint16_t reserved;
	union
	{
		multiboot_framebuffer_rgb rgb;
		multiboot_framebuffer_indexed indexed;
	};
} __attribute__((packed)) multiboot_tag_framebuffer;

#pragma GCC diagnostic pop

#ifdef __cplusplus
}
#endif
