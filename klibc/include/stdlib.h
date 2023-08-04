#pragma once

#ifdef __cplusplus
extern "C" {
#endif

long lrand48(void);
long nrand48(unsigned short xsubi[3]);

long mrand48(void);
long jrand48(unsigned short xsubi[3]);

void srand48(long seedval);
unsigned short* seed48(unsigned short seed16v[3]);
void lcong48(unsigned short param[7]);

#ifdef __cplusplus
}
#endif
