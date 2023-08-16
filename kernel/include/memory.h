#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define KERNEL_VMA 0xFFFFFF8000000000ULL
#define USER_BREAK 0x0000008000000000ULL

// Memory Sizes

#define KiB 1024ULL
#define MiB 1048576ULL
#define GiB 1073741824ULL
#define TiB 1099511627776ULL

// Virtual <-> Physical conversion

#define V2P(addr) ((uintptr_t)(addr) & ~KERNEL_VMA)
#define P2V(addr) ((uintptr_t)(addr) | KERNEL_VMA)

#ifdef __cplusplus
}
#endif
