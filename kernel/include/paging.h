#pragma once

#include <stdint.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PAGE_SIZE		4096U
#define LARGE_PAGE_SIZE	2097152U
#define HUGE_PAGE_SIZE	1073741824ULL

#define PML4_ENTRIES 512U
#define PML3_ENTRIES 512U
#define PML2_ENTRIES 512U
#define PML1_ENTRIES 512U

#ifndef __ASSEMBLER__

typedef struct pml4_entry
{
	uint64_t present			: 1;
	uint64_t read_write			: 1;
	uint64_t user				: 1;
	uint64_t write_through		: 1;
	uint64_t cache_disable		: 1;
	uint64_t accessed			: 1;
	uint64_t ignored_0			: 1;
	uint64_t huge_page			: 1;
	uint64_t ignored_1			: 4;
	uint64_t ppn				: 28;
	uint64_t reserved			: 12;
	uint64_t ignored_2			: 11;
	uint64_t execute_disable	: 1;
} __attribute__((packed)) pml4_entry;

typedef struct pml3_entry
{
	uint64_t present			: 1;
	uint64_t read_write			: 1;
	uint64_t user				: 1;
	uint64_t write_through		: 1;
	uint64_t cache_disable		: 1;
	uint64_t accessed			: 1;
	uint64_t ignored_0			: 1;
	uint64_t huge_page			: 1;
	uint64_t ignored_1			: 4;
	uint64_t ppn				: 28;
	uint64_t reserved			: 12;
	uint64_t ignored_2			: 11;
	uint64_t execute_disable	: 1;
} __attribute__((packed)) pml3_entry;

typedef struct pml2_entry
{
	uint64_t present			: 1;
	uint64_t read_write			: 1;
	uint64_t user				: 1;
	uint64_t write_through		: 1;
	uint64_t cache_disable		: 1;
	uint64_t accessed			: 1;
	uint64_t ignored_0			: 1;
	uint64_t huge_page			: 1;
	uint64_t ignored_1			: 4;
	uint64_t ppn				: 28;
	uint64_t reserved			: 12;
	uint64_t ignored_2			: 11;
	uint64_t execute_disable	: 1;
} __attribute__((packed)) pml2_entry;

typedef struct pml1_entry
{
	uint64_t present			: 1;
	uint64_t read_write			: 1;
	uint64_t user				: 1;
	uint64_t write_through		: 1;
	uint64_t cache_disable		: 1;
	uint64_t accessed			: 1;
	uint64_t dirty				: 1;
	uint64_t pat				: 1;
	uint64_t global				: 1;
	uint64_t ignored_0			: 3;
	uint64_t ppn				: 28;
	uint64_t reserved			: 12;
	uint64_t ignored_1			: 7;
	uint64_t protection_key		: 4;
	uint64_t execute_disable	: 1;
} __attribute__((packed)) pml1_entry;

// For 1 GiB pages
typedef struct pml3_huge_entry
{
	uint64_t present			: 1;
	uint64_t read_write			: 1;
	uint64_t user				: 1;
	uint64_t write_through		: 1;
	uint64_t cache_disable		: 1;
	uint64_t accessed			: 1;
	uint64_t dirty				: 1;
	uint64_t huge_page			: 1;
	uint64_t global				: 1;
	uint64_t ignored_0			: 3;
	uint64_t pat				: 1;
	uint64_t reserved_0			: 17;
	uint64_t ppn				: 10;
	uint64_t reserved_1			: 12;
	uint64_t ignored_1			: 7;
	uint64_t protection_key		: 4;
	uint64_t execute_disable	: 1;
} __attribute__((packed)) pml3_huge_entry;

// For 2 MiB pages
typedef struct pml2_huge_entry
{
	uint64_t present			: 1;
	uint64_t read_write			: 1;
	uint64_t user				: 1;
	uint64_t write_through		: 1;
	uint64_t cache_disable		: 1;
	uint64_t accessed			: 1;
	uint64_t dirty				: 1;
	uint64_t huge_page			: 1;
	uint64_t global				: 1;
	uint64_t ignored_0			: 3;
	uint64_t pat				: 1;
	uint64_t reserved_0			: 8;
	uint64_t ppn				: 19;
	uint64_t reserved_1			: 12;
	uint64_t ignored_1			: 7;
	uint64_t protection_key		: 4;
	uint64_t execute_disable	: 1;
} __attribute__((packed)) pml2_huge_entry;

static_assert(sizeof(pml4_entry) == sizeof(uint64_t), "PML4 page entry has to be 64 bit wide");
static_assert(sizeof(pml3_entry) == sizeof(uint64_t), "PML3 page entry has to be 64 bit wide");
static_assert(sizeof(pml2_entry) == sizeof(uint64_t), "PML2 page entry has to be 64 bit wide");
static_assert(sizeof(pml1_entry) == sizeof(uint64_t), "PML1 page entry has to be 64 bit wide");
static_assert(sizeof(pml3_huge_entry) == sizeof(uint64_t), "PML3 (huge) page entry has to be 64 bit wide");
static_assert(sizeof(pml2_huge_entry) == sizeof(uint64_t), "PML2 (huge) page entry has to be 64 bit wide");

#endif

#ifdef __cplusplus
}
#endif
