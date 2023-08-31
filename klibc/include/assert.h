#pragma once


#ifdef __cplusplus
extern "C" {
#endif

#ifndef static_assert
#define static_assert(expression, message) _Static_assert(expression, message)
#endif

#ifndef NDEBUG

#define assert(expression) \
if (!(expression)) \
{ \
    debugf("\033[91mASSERTION FAILED\033[m at \033[94m%s\033[m:\033[93m%u\033[m:%s\033[m: '\033[92m%s\033[m'\n", \
	__FILE__, __LINE__, __func__, #expression); \
    asm volatile("int3"); \
} else ((void)0)

#elif

#define assert(expression) ((void)0)

#endif

#ifdef __cplusplus
}
#endif
