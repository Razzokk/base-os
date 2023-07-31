#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STRINGIFY(x) #x
#define XSTRINGIFY(x) STRINGIFY(x)

uint64_t rdtsc();

void hw_cursor_disable();
void hw_cursor_enable(size_t cursor_start, size_t cursor_end);
void hw_cursor_set_pos(size_t row, size_t col);
void hw_cursor_get_pos(size_t* row, size_t* col);

#ifdef __cplusplus
}
#endif
