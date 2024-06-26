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

void ffge_32i8_modprim(int64_t *in, int32_t *out)
{
	for (size_t i = 0; i < WIDTH / 2; i++) {
		out[i] = (int32_t)(in[2*i] % FFGE_MAGPRIM);
		out[i + WIDTH] = (int32_t)(in[2*i + 1] % FFGE_MAGPRIM);
	}
}

int32_t modmagprim(int64_t a)
{
	uint64_t  x, y, z, c, r;

	int8_t s = 1;
	if (a < 0) {
		a = -a;
		s = -1;
	}

	x = a				& FFGE_MAGPRIM;
	y = (a >> 31)			& FFGE_MAGPRIM;
	z = (a >> 62)			& FFGE_MAGPRIM;
	c = ((x + y + z) >> 31)		& 1; // carry
	r = (x + y + z + c)		& FFGE_MAGPRIM;

	if (r == FFGE_MAGPRIM)
		r = 0;

	return (int32_t)r * s;
}
