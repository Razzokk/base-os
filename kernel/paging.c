#include "paging.h"
#include "memory.h"

pml4_table_t* current_address_space = NULL;

void set_pml4(pml4_table_t* pml4_table)
{
	current_address_space = pml4_table;
	asm volatile("mov %[pml4], %%cr3" :: [pml4]"d"(V2P(pml4_table)));
}
