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
uint64_t ffge_pivmtr_64i(size_t n, int64_t *m,
				 size_t pr, size_t pc, uint32_t w)
{
	uint64_t fl = 0;
	for (size_t k = 0; k < w; k++) {
		size_t i = pr;
		while (i < n && m[(i*n + pc)*w + k] == 0)
			i++;
		if (i == n) {
			fl |= UINT64_C(1) << k;
			continue;
		}
		if (i > pr)
			for (size_t j = pc; j < n; j++) {
				const size_t rj = (pr*n + j)*w + k;
				const size_t ij = ( i*n + j)*w + k;

				const int64_t x = m[rj];
				m[rj] = m[ij];
				m[ij] = x;
			}
	}

	return fl;
}

int ffge_64i1(size_t n, int64_t *m, size_t *rnk)
{
	size_t pc = 0, pr = 0;	// pivot col, row
	for (pc = 0; pc < n; pc++) {
		if (ffge_pivmtr_64i(n, m, pr, pc, 1) != 0)
			continue;
		for (size_t i = pr + 1; i < n; i++) {
			for (size_t j = pc + 1; j < n; j++ ){
				m[i*n + j] = (
						m[pr*n + pc] * m[i *n + j] -
						m[i *n + pc] * m[pr*n + j]
					) % FFGE_MAGPRIM;
			}
			m[i*n + pc] = 0;
		}
		pr++;
	}
	if (rnk)
		*rnk = pr;

	return pr == n;
}

