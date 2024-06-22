/* -------------------------------------------------------------------------- *
 * bench-ffge.c: Benchmark for a raw implementation of FFGE.                  *
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
#define _XOPEN_SOURCE 700

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "flint/fmpz.h"
#include "flint/fmpz_mat.h"

#include "ffge.h"

#define SIZE (12)
#define REPS (1<<14)

#define TIMEIT(acc, x) ({ 					\
		struct timespec b, e;				\
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &b);	\
		(x);						\
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &e);	\
		acc += e.tv_nsec - b.tv_nsec;			\
	})
#define MSREP(acc) ((double)acc * 1.0e-3 / REPS)

static int32_t A0[REPS * SIZE * SIZE];
static int32_t *A[REPS][SIZE];
static fmpz_mat_t B[REPS];

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	/* Initialize matrices to random integers: -1, 0, 1 */
	for(size_t r = 0; r < REPS; r++) {
    		fmpz_mat_init(B[r], SIZE, SIZE);
		for (size_t i = 0; i < SIZE; i++) {
			for (size_t j = 0; j < SIZE; j++) {
				int32_t a = (int32_t)rand() % 3 - 1 ;
				A0[(r * SIZE +  i) * SIZE + j] = a;
				fmpz_set_si(fmpz_mat_entry(B[r], i, j), a);
			}
			A[r][i] = A0 + (r * SIZE + i) * SIZE;
		}
	}

	/*
	 * Measure the total time spent on LU.
	 * Assert our implementation gives correct value.
	 */
	uint32_t ta = 0, tb = 0;
	for (size_t r = 0; r < REPS; r++) {
		int rta, rtb;

		TIMEIT(ta, rta = ffge32(SIZE, A[r]));
		TIMEIT(tb, rtb = fmpz_mat_rank(B[r]));

		rtb = rtb == SIZE ? 0 : -1;
		assert(rta == rtb);
	}
	for (size_t r = 0; r < REPS; r++)
		fmpz_mat_clear(B[r]);

	printf("size: %d, reps: %d\n", SIZE, REPS);
	printf("fflu(A)           %.3f μs\n", MSREP(ta));
	printf("fmpz_mat_fflu(B)  %.3f μs\n", MSREP(tb));

	return 0;
}

