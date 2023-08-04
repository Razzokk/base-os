#include "rand.h"

#include <stdlib.h>

void seed_rand(uint64_t seed)
{
	srand48(seed);
}

uint64_t rand(void)
{
	return lrand48();
}
