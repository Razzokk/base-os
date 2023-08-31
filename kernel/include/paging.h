#pragma once

#include <stdint.h>
#include <assert.h>
#include <stddef.h>
#include "memory.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PAGE_SIZE		(4*KiB)
#define LARGE_PAGE_SIZE	(2*MiB)
#define HUGE_PAGE_SIZE	(1*GiB)

#define NUM_PML4_ENTRIES 512ULL
#define NUM_PML3_ENTRIES 512ULL
#define NUM_PML2_ENTRIES 512ULL
#define NUM_PML1_ENTRIES 512ULL

#define addr2page(addr, page_size) ((uintptr_t) addr / page_size)

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

typedef pml4_entry pml4_table_t[NUM_PML4_ENTRIES];
typedef pml3_entry pml3_table_t[NUM_PML3_ENTRIES];
typedef pml2_entry pml2_table_t[NUM_PML2_ENTRIES];
typedef pml1_entry pml1_table_t[NUM_PML1_ENTRIES];
typedef pml3_huge_entry pml3_huge_table_t[NUM_PML3_ENTRIES];
typedef pml2_huge_entry pml2_huge_table_t[NUM_PML2_ENTRIES];

static_assert(sizeof(pml4_entry) == sizeof(uint64_t), "PML4 page entry has to be 64 bit wide");
static_assert(sizeof(pml3_entry) == sizeof(uint64_t), "PML3 page entry has to be 64 bit wide");
static_assert(sizeof(pml2_entry) == sizeof(uint64_t), "PML2 page entry has to be 64 bit wide");
static_assert(sizeof(pml1_entry) == sizeof(uint64_t), "PML1 page entry has to be 64 bit wide");
static_assert(sizeof(pml3_huge_entry) == sizeof(uint64_t), "PML3 (huge) page entry has to be 64 bit wide");
static_assert(sizeof(pml2_huge_entry) == sizeof(uint64_t), "PML2 (huge) page entry has to be 64 bit wide");
static_assert(sizeof(pml4_table_t) == PAGE_SIZE, "PML4 table has to be of size PAGE_SIZE (4096 bytes)");
static_assert(sizeof(pml3_table_t) == PAGE_SIZE, "PML3 table has to be of size PAGE_SIZE (4096 bytes)");
static_assert(sizeof(pml2_table_t) == PAGE_SIZE, "PML2 table has to be of size PAGE_SIZE (4096 bytes)");
static_assert(sizeof(pml1_table_t) == PAGE_SIZE, "PML1 table has to be of size PAGE_SIZE (4096 bytes)");
static_assert(sizeof(pml3_huge_table_t) == PAGE_SIZE, "PML3 (huge) table has to be of size PAGE_SIZE (4096 bytes)");
static_assert(sizeof(pml2_huge_table_t) == PAGE_SIZE, "PML2 (huge) table has to be of size PAGE_SIZE (4096 bytes)");

extern pml4_table_t* current_address_space;

void set_pml4(pml4_table_t* pml4_table);

#ifdef __cplusplus
}
#endif
