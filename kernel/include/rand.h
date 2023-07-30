#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void seed_rand(uint64_t seed);

uint64_t rand();

#ifdef __cplusplus
}
#endif
