#include <stdlib.h>

#include <stdint.h>

#define _DEFAULT_PARAM_A_0 0xE66DULL
#define _DEFAULT_PARAM_A_1 0xDEECULL
#define _DEFAULT_PARAM_A_2 0x0005ULL
#define _DEFAULT_PARAM_C 0xBULL
#define _PARAM_M (1ULL << 48)

#define _LOWER_16_BITS 0x330E

unsigned short _rand48_xsubi[3] = { 0 };
uint64_t _rand48_a = _DEFAULT_PARAM_A_2 << 32 | _DEFAULT_PARAM_A_1 << 16 | _DEFAULT_PARAM_A_0;
unsigned short _rand48_c = _DEFAULT_PARAM_C;

static unsigned short _buffer[3];

// stdlib
long lrand48(void)
{
	return nrand48(_rand48_xsubi);
}

long nrand48(unsigned short xsubi[3])
{
	return jrand48(xsubi) & INT32_MAX;
}

long mrand48(void)
{
	return jrand48(_rand48_xsubi);
}

long jrand48(unsigned short xsubi[3])
{
	uint64_t x = (uint64_t)xsubi[2] << 32 | xsubi[1] << 16 | xsubi[0];

	x = (_rand48_a * x + _rand48_c) % _PARAM_M;

	xsubi[0] = x & 0xFFFF;
	xsubi[1] = (x >> 16) & 0xFFFF;
	xsubi[2] = (x >> 32) & 0xFFFF;

	return (int32_t)xsubi[2] << 16 | xsubi[1];
}

void srand48(long seedval)
{
	unsigned short param[7] =
			{
					_LOWER_16_BITS,
					seedval & 0xFFFF,
					(seedval >> 16) & 0xFFFF,
					_DEFAULT_PARAM_A_0,
					_DEFAULT_PARAM_A_1,
					_DEFAULT_PARAM_A_2,
					_DEFAULT_PARAM_C
			};

	lcong48(param);
}

unsigned short* seed48(unsigned short seed16v[3])
{
	_buffer[0] = _rand48_xsubi[0];
	_buffer[1] = _rand48_xsubi[1];
	_buffer[2] = _rand48_xsubi[2];

	unsigned short param[7] =
			{
					seed16v[0],
					seed16v[1],
					seed16v[2],
					_DEFAULT_PARAM_A_0,
					_DEFAULT_PARAM_A_1,
					_DEFAULT_PARAM_A_2,
					_DEFAULT_PARAM_C
			};

	lcong48(param);

	return _buffer;
}

void lcong48(unsigned short param[7])
{
	_rand48_xsubi[0] = param[0];
	_rand48_xsubi[1] = param[1];
	_rand48_xsubi[2] = param[2];

	_rand48_a = (uint64_t)param[5] << 32 | param[4] << 16 | param[3];

	_rand48_c = param[6];
}
