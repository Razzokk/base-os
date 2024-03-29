#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Alignment

#if !__has_builtin(__builtin_is_aligned)
#define __builtin_is_aligned(x, align) ((((uintptr_t) x) & ((align) - 1)) == 0)
#endif

#if !__has_builtin(__builtin_align_up)
#define __builtin_align_up(x, align) ((__typeof__(x)) ((((uintptr_t) x) + ((align) - 1)) & (~((align) - 1))))
#endif

#if !__has_builtin(__builtin_align_down)
#define __builtin_align_down(x, align) ((__typeof__(x)) ((x) & (~((align) - 1))))
#endif

#define align_up(x, align) __builtin_align_up(x, align)
#define align_down(x, align) __builtin_align_down(x, align)
#define is_aligned(x, align) __builtin_is_aligned(x, align)

/** Generic function pointer type */
typedef void(*fptr_t)(void);

#define offset_pointer(ptr, offset) ((__typeof__(ptr)) (((uintptr_t) ptr) + offset))

#ifdef __cplusplus
}
#endif
