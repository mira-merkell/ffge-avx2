#include "stdint.h"

#include "ffge.h"

#define WIDTH (8)

uint64_t ffge_32i8_pivot(size_t n, int32_t *m, size_t pr, size_t pc)
{
	uint64_t fl = 0;
	for (size_t k = 0; k < WIDTH; k++) {
		size_t i = pr;
		while (i < n && m[(i*n + pc)*WIDTH + k] == 0)
			i++;
		if (i == n) {
			fl |= UINT64_C(1) << k;
			continue;
		}
		if (i > pr)
			for (size_t j = pc; j < n; j++) {
				const size_t rj = (pr*n + j)*WIDTH + k;
				const size_t ij = ( i*n + j)*WIDTH + k;

				const int32_t x = m[rj];
				m[rj] = m[ij];
				m[ij] = x;
			}
	}

	return fl;
}

int32_t ffge_32i8_modprim(int64_t a)
{
	int8_t s, o;
	uint64_t  b, c, r;

	s = a >= 0 ? 1 : -1;
	b = (uint64_t)(a * s);

	r  = b >> 62;
	r += b <<  2 >> 33;
	r += b << 33 >> 33;
	c  = 1 << 31  & r;
	r ^= c;
	r += c >> 31;

	o = r == FFGE_MAGPRIM ? 0 : 1;

	return (int32_t)r * s * o;
}

