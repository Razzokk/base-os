#pragma once

#include <stddef.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

int sprintf(char* str, const char* format, ...);
int snprintf(char* str, size_t size, const char* format, ...);

int vsprintf(char* str, const char* format, va_list args);
int vsnprintf(char* str, size_t size, const char* format, va_list args);

#ifdef __cplusplus
}
#endif
