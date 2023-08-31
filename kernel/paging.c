#include "paging.h"
#include "memory.h"

pml4_entry* current_address_space = NULL;

void set_pml4(pml4_entry* pml4_table)
{
	current_address_space = pml4_table;
	asm volatile("mov %[pml4], %%cr3" :: [pml4]"d"(V2P(pml4_table)));
}
