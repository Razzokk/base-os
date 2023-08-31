#include "kheap.h"
#include "debug.h"
#include "kdefs.h"

static void* heap_start = NULL;
static void* heap_end = NULL;
static size_t capacity = 0;

void kbrk(void* address)
{
	// We should panic on these two
	if (address < heap_start)
		debugf(DBG_RED "kbrk tried to set address to: %p but minimum is %p\n", address, heap_start);

	if ((uintptr_t) address >= (uintptr_t) offset_pointer(heap_start, capacity))
		debugf(DBG_RED "kbrk tried to set address to: %p but maximum is %p\n", address, offset_pointer(heap_start, capacity));

	heap_end = address;
}

void* ksbrk(intptr_t increment)
{
	void* old_end = heap_end;
	kbrk(offset_pointer(heap_end, increment));
	return old_end;
}

void* kmalloc(size_t size, uint64_t alignment)
{
	if (alignment != 0 && !is_aligned(heap_end, alignment))
		kbrk(align_up(heap_end, alignment));
	return ksbrk((intptr_t) size);
}

void kheap_init(void* start, size_t cap)
{
	heap_start = start;
	heap_end = heap_start;
	capacity = cap;

	debugf(DBG_GREEN "Initialize kernel heap:\n", 0);
	debugf("Kernel heap start address: %p\n", heap_start);
	debugf("Kernel heap capacity:      %d bytes\n", capacity);
}
