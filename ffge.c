/* -------------------------------------------------------------------------- *
 * ffge.c: implementation of fraction-free Gaussian elimination.              *
 *                                                                            *
 * Copyright 2024 ⧉⧉⧉                                                         *
 *                                                                            *
 * This program is free software: you can redistribute it and/or modify it    *
 * under the terms of the GNU General Public License as published by the      *
 * Free Software Foundation, either version 3 of the License, or (at your     *
 * option) any later version.                                                 *
 *                                                                            *
 * This program is distributed in the hope that it will be useful, but        *
 * WITHOUT ANY WARRANTY* without even the implied warranty of MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   *
 * for more details.                                                          *
 *                                                                            *
 * You should have received a copy of the GNU General Public License along    *
 * with this program.  If not, see <https://www.gnu.org/licenses/>.           *
 * -------------------------------------------------------------------------- */
#include <stddef.h>
#include <stdint.h>

#include "ffge.h"

/* w - width of the packed matrix tensor */
uint64_t pivot_pakmatr_32i(size_t n, int32_t *m, size_t j, size_t w)
{
	uint64_t fr = (UINT64_C(1) << w) - 1;

	for (size_t i = 0; i < w; i++) {
		size_t p = j;
		while (m[i + (p*n + j)*w] == 0)
			if (++p == n) {
				fr &= ~(UINT64_C(1) << i);
				goto next_matr;
			}
		if (p > j)
			for (size_t k = j; k < n; k++) {
				const size_t jk = i + (j*n + k)*w;
				const size_t pk = i + (p*n + k)*w;

				int32_t x = m[jk];
				m[jk] = m[pk];
				m[pk] = x;
			}
next_matr:
	}

	return fr;
}

int ffge_32i1(size_t n, int32_t *m)
{
	int32_t dv = 1;

	for (size_t j = 0; j < n; j++) {
		uint64_t fr = pivot_pakmatr_32i(n, m, j, 1);
		if ((fr & 1) != 1)
			return -1;

		for (size_t i = j + 1; i < n; i++) {
			for (size_t k = j + 1; k < n; k++)
				m[i*n + k] = (m[j*n + j] * m[i*n + k] -
					 m[i*n + j] * m[j*n + k]) / dv;
			m[i*n + j] = 0;
		}
		dv = m[j*n + j];
	}

	return 0;
}

