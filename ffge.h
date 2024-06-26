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

/* Magic prime: 2^31 - 1. Integer multiplication fits into int64_t. */
#define FFGE_MAGPRIM (2147483647)

/* -------------------------------------------------------------------------- *
 * Perform in-place fraction-free Gaussian elimination on matrix m            *
 * of size n.  The underlying field is integers mod FFGE_MAGPRIM.             *
 *                                                                            *
 * The matrix is represented as a continuous array of n*n elements.           *
 * The matrix element e = m_ij, for 0 <= i,j < n, is accessed with:           *
 *                                                                            *
 *      int32_t e = m[i*n + j];                                               *
 *                                                                            *
 * If the pointer rnk is not null, the rank of m is stored.                   *
 *                                                                            *
 * Returns:                                                                   *
 *      0       - if the matrix is singular                                   *
 *      1       - if the matrix is full-rank                                  *
 * -------------------------------------------------------------------------- */
int ffge_32i1(size_t n, int32_t *m, size_t *rnk);

/* -------------------------------------------------------------------------- *
 * Perform in-place fraction-free Gaussian elimination on 8 matrices m        *
 * of size n. The underlying field is integers mod FFGE_MAGPRIM.              *
 *                                                                            *
 * The matrices are represented together as a continuous array                *
 * of lenth n*n*8.                                                            *
 *                                                                            *
 *           +++ We assume the array m is aligned to 32 bytes. +++            *
 *                                                                            *
 * If 0 <= i, j < n, and 0 <= k < 8, then m[(i * n + j)*8 + k] is the         *
 * (i,j)-element of the k-th matrix.                                          *
 *                                                                            *
 * The procedure gives the correct result only for non-singular matrices.     *
 * If the matrix is singular, it's destroyed and the last diagonal element    *
 * is set to zero.                                                            *
 *                                                                            *
 * Retruns:                                                                   *
 *      0       - if all matrices are singular                                *
 *      1       - if at least one matrix is full-rank                         *
 * -------------------------------------------------------------------------- */
int ffge_32i8(size_t n, int32_t *m);

#endif /* FFGE_H */
