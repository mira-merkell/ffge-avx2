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

/*
 * Perform in-place fraction-free Gaussian elimination on matrix m
 * of size n.  Short-circuit if m is not full-rank.
 *
 * The matrix is represented as an array of pointers to rows,
 * for faster pivoting.
 *
 * Returns:
 *	 0	- m is full rank
 * 	-1	- otherwise
 *
 */
int ffge32(size_t n, int32_t **m);

#endif /* FFGE_H */
