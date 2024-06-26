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

#include <flint/fmpz.h>
#include <flint/fmpz_mat.h>

#include "ffge.h"
#include "xoshiro256ss.h"

#define SEED (1772)
static struct xoshiro256ss RNG;
#define RAND_PM(ptr)	({						\
		*ptr = (int8_t) (xoshiro256ss_next(&RNG) % 3 - 1);	\
	})


/* Keep the number of repetitions divisible by 4 */
#define REPS (1<<16)
#define SIZE (12)

#define TIMEIT(acc, x) ({ 						\
		struct timespec b, e;					\
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &b);		\
		(x);							\
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &e);		\
		acc += e.tv_nsec - b.tv_nsec;				\
	})
#define MUSREP(acc) ((double)acc * 1.0e-3 / REPS)

static fmpz_mat_t A;
static int32_t B[SIZE * SIZE];
#define WIDTH	(8)
static _Alignas(32) int32_t C[SIZE * SIZE * WIDTH];

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	xoshiro256ss_init(&RNG, SEED);

	/* Initialize matrices to random integers: -1, 0, 1.
	 *
	 * Measure the total time spent on LU.
	 * Assert our implementation gives correct value.
	 */
	uint64_t ta, tb, tc;
	ta = tb = tc = 0;
	for (size_t r = 0; r < REPS; r++) {
		volatile uint64_t xx = 0;

		xx++;

		fmpz_mat_init(A, SIZE, SIZE);
		for (size_t i = 0; i < SIZE; i++) {
			for (size_t j = 0; j < SIZE; j++) {
				int32_t a;
				RAND_PM(&a);

				B[i*SIZE + j] = a;
				fmpz_set_si(fmpz_mat_entry(A, i, j), a);
				for (size_t k = 0; k < WIDTH; k++) {
					int32_t ak;
					RAND_PM(&ak);

					C[(i*SIZE + j)*WIDTH + k] = ak;
				}
			}
		}

		xx++;

		long rta;
		TIMEIT(ta, rta = fmpz_mat_rank(A));

		xx++;

		size_t rnk;
		int rtb;
		TIMEIT(tb, rtb = ffge_32i1(SIZE, B, &rnk));
		(void)rtb;

		xx++;

		int rtc;
		TIMEIT(tc, rtc = ffge_32i8(SIZE, C));
		(void)rtc;

		xx++;

		fmpz_mat_clear(A);
		assert((size_t)rta == rnk);
	}

	printf("size: %d, reps: %d\n", SIZE, REPS);
	printf("\tfmpz_mat_rank(A)   %.3f μs\n", MUSREP(ta));
	printf("\tffge_64i1(B)       %.3f μs\n", MUSREP(tb));
	printf("\tffge_64i8(C)       %.3f μs\n", MUSREP(tc) / WIDTH);

	return 0;
}

