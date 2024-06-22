/* -------------------------------------------------------------------------- *
 * ffge.h: implementation of fraction-free Gaussian elimination.              *
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
#ifndef FFGE_H
#define FFGE_H

#include <stddef.h>
#include <stdint.h>

#include <x86intrin.h>

/*
 * Perform in-place fraction-free Gaussian elimination on matrix m
 * of size n.  Short-circuit if m is not full-rank.
 *
 * The matrix is represented as a continuous array of n*n elements.
 * Hence, the matrix element e = m_ij, for 0 <= i,j < n, is accessed with:
 *
 *	int32_t e = m[i*n + j];
 *
 * Returns:
 *	 0	- m is full rank
 * 	-1	- otherwise
 *
 */
int ffge32(size_t n, int32_t *m);

/*
 * Perform in-place fraction-free Gaussian elimination on 8 matrices m
 * of size n.
 *
 * The matrices are represented together as a continuous array of lenth n*n
 * of packed double word integers: __m256i.  Hence, elements of each
 * matrix are 32-bit signed integers.
 *
 * If 0 <= i, j < n, and 0 <= k < 8, then m[i * n + j][k] is the
 * (i,j)-element of the k-th matrix.  You can access them like this:
 *
 * 	_Alignas(32) int32_t e[8];
 *	_mm256_store_si256((__m256 *)e, m[i][j]);
 *
 * If one of the matrices, say the k-th one, is singular, then the k-th
 * bit in fr (full-rank) is cleared the procedure aborts for this particular
 * matrix.  Other matrices are processed further. Bits in fr beyond 0xff are
 * unaffected.
 *
 * To check if any of the matrices is full-rank, simply compare the value of
 * 8 lower bits of fr to zero:
 *
 *	int fr;
 *	ffge_32i8(SIZE, m, &fr);
 *	if (fr & 0xff)
 *		printf("full-rank!\n");
 *
 * Retruns:
 * 	 0	- if all matrices in m are full-rank
 *	-1	- otherwise (i.e at least one matrix is singular)
 *
 */
int ffge_32i8(size_t n, __m256i *m, uint64_t *fr);

#endif /* FFGE_H */