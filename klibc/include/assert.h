#pragma once


#ifdef __cplusplus
extern "C" {
#endif

#ifndef static_assert
#define static_assert(expression, message) _Static_assert(expression, message)
#endif


#ifdef __cplusplus
}
#endif
