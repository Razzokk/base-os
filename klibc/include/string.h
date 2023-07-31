#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void* ptr1, const void* ptr2, size_t n);

void* memcpy(void* dest, const void* source, size_t n);

void* memmove(void* dest, const void* source, size_t n);

void* memset(void* ptr, int value, size_t n);

int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, size_t n);

char* strcpy(char* dest, const char* source);
char* strncpy(char* dest, const char* source, size_t n);

size_t strlen(const char* str);
size_t strnlen(const char* str, size_t n);

#ifdef __cplusplus
}
#endif
