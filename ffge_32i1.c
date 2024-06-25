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

uint64_t ffge_32i1_pivot(size_t n, int32_t *m, size_t pr, size_t pc)
{
	size_t i = pr;
	while (i < n && m[i*n + pc] == 0)
		i++;
	if (i == n)
		return 1;
	if (i > pr)
		for (size_t j = pc; j < n; j++) {
			const size_t rj = pr*n + j;
			const size_t ij =  i*n + j;

			const int32_t x = m[rj];
			m[rj] = m[ij];
			m[ij] = x;
		}

	return 0;
}

int ffge_32i1(size_t n, int32_t *m, size_t *rnk)
{
	size_t pc = 0, pr = 0;	// pivot col, row
	for (pc = 0; pc < n; pc++) {
		if (ffge_32i1_pivot(n, m, pr, pc) != 0)
			continue;

		const int64_t m_rc = m[pr*n + pc];
		for (size_t i = pr + 1; i < n; i++) {
			const int64_t m_ic = m[i*n + pc];
			for (size_t j = pc + 1; j < n; j++ ){
				const int64_t m_ij = m[i*n + j];
				const int64_t m_rj = m[pr*n + j];

				const int64_t x = m_rc * m_ij - m_ic * m_rj;
				m[i*n + j] = (int32_t)(x % FFGE_MAGPRIM);
			}
			m[i*n + pc] = 0;
		}
		pr++;
	}
	if (rnk)
		*rnk = pr;

	return pr == n;
}

