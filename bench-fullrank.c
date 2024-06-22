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

#include <x86intrin.h>

#include "flint/fmpz.h"
#include "flint/fmpz_mat.h"

#include "ffge.h"

#define SEED (1772)

/* Keep the number of repetitions divisible by 8 */
#define REPS (1<<14)
#define SIZE (12)

#define TIMEIT(acc, x) ({ 					\
		struct timespec b, e;				\
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &b);	\
		(x);						\
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &e);	\
		acc += e.tv_nsec - b.tv_nsec;			\
	})
#define MUSREP(acc) ((double)acc * 1.0e-3 / REPS)

static fmpz_mat_t A[REPS];
static int32_t B[REPS][SIZE * SIZE];
static __m256i C[REPS / 8][SIZE * SIZE];

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	srand(SEED);

	/* Initialize matrices to random integers: -1, 0, 1 */
	for(size_t r = 0; r < REPS; r++) {
    		fmpz_mat_init(A[r], SIZE, SIZE);
		for (size_t i = 0; i < SIZE; i++) {
			for (size_t j = 0; j < SIZE; j++) {
				int32_t a = (int32_t)(rand() % 3) - 1;
				B[r][i*SIZE + j] = a;
				fmpz_set_si(fmpz_mat_entry(A[r], i, j), a);
			}
		}
	}
	for (size_t r = 0; r < REPS/8; r++) {
		for (size_t i = 0; i < SIZE; i++) {
			for (size_t j = 0; j < SIZE; j++) {
				_Alignas(32) int32_t v[8];
				for (size_t k = 0; k < 8; k++)
					v[k] = B[r*8 + k][i*SIZE + j];
				C[r][i*SIZE + j] =
					 _mm256_load_si256((__m256i *)v);
			}
		}
	}

	/*
	 * Measure the total time spent on LU.
	 * Assert our implementation gives correct value.
	 */
	uint64_t ta, tb, tc;
	ta = tb = tc = 0;
	for (size_t r = 0; r < REPS; r++) {
		int rta, rtb, rtc;

		TIMEIT(ta, rta = fmpz_mat_rank(A[r]));
		TIMEIT(tb, rtb = ffge_32i(SIZE, B[r]));

		rta = rta == SIZE ? 0 : -1;
		assert(rta == rtb);

		uint64_t fr;
		(void) rtc;
		if (r % 8 == 0)
			TIMEIT(tc, rtc = ffge_32i8(SIZE, C[r/8], &fr));
	}
	for (size_t r = 0; r < REPS; r++)
		fmpz_mat_clear(A[r]);

	printf("size: %d, reps: %d\n", SIZE, REPS);
	printf("\tfmpz_mat_rank(A)   %.3f μs\n", MUSREP(ta));
	printf("\tffge_32i(B)        %.3f μs\n", MUSREP(tb));
	printf("\tffge_32i8(C)       %.3f μs\n", MUSREP(tc));

	return 0;
}

