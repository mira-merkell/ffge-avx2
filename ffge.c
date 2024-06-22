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

int ffge32(size_t n, int32_t **m)
{
	int32_t dv = 1;

	for (size_t j = 0; j < n; j++) {
		/* Pivot rows if needed */
		size_t p = j;
		while (m[p][j] == 0)
			if (++p == n)
				return -1;
		if (p > j) {
			int32_t *r = m[j];
			m[j] = m[p];
			m[p] = r;
		}

		for (size_t i = j + 1; i < n; i++) {
			for (size_t k = j + 1; k < n; k++)
				m[i][k] = (m[j][j] * m[i][k] -
					 m[i][j] * m[j][k]) / dv;
			m[i][j] = 0;
		}
		dv = m[j][j];
	}

	return 0;
}

