#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define KERNEL_VMA 0xFFFFFF8000000000
#define USER_BREAK 0x0000008000000000

// Virtual <-> Physical conversion

#define V2P(addr) ((uintptr_t)(addr) & ~KERNEL_VMA)
#define P2V(addr) ((uintptr_t)(addr) | KERNEL_VMA)

#ifdef __cplusplus
}
#endif
