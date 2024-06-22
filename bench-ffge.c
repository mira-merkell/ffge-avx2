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

#include "fmpz.h"
#include "fmpz_mat.h"

#define SIZE (12)
#define REPS (10000)

int ffge(size_t n, int64_t **m);


static int64_t A0[SIZE * SIZE];
static int64_t *A[SIZE];

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	struct timespec beg, end;
	uint64_t lpsA = 0, lpsB = 0;
	int singA, singB;

	fmpz_t den;
    	fmpz_mat_t B;
    	fmpz_mat_init(B, SIZE, SIZE);

	for(size_t r = 0; r < REPS; r++) {
		/* Initialize matrices */
		for (size_t i = 0; i < SIZE; i++) {
			A[i] = A0 + SIZE * i;
			for (size_t j = 0; j < SIZE; j++) {
				int64_t a = (int64_t)rand() % 8;
				fmpz_set_si(fmpz_mat_entry(B, i, j), a);
				A[i][j] = a;
			}
		}

#define TIMEIT(x) ({ 						\
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &beg);	\
		(x);						\
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);	\
	})

		TIMEIT(singA = ffge(SIZE, A));
		lpsA += end.tv_nsec - beg.tv_nsec;

		TIMEIT(singB = fmpz_mat_fflu(B, den, (void *)0, B, 1));
		lpsB += end.tv_nsec - beg.tv_nsec;

		if (singA != singB)
			printf("assert error: singA=%d, singB=%d\n",
				singA, singB);
	}
	fmpz_mat_clear(B);

	printf("         fflu(A), %.3f ms / rep\n",
			(double)lpsA * 1.0e-3 / REPS);
	printf("fmpz_mat_fflu(B), %.3f ms / rep\n",
			(double)lpsB * 1.0e-3 / REPS);

	return 0;
}


int ffge(size_t n, int64_t **m)
{
	for (size_t j = 0; j < n ; j++) {

		size_t p = j;
		while (m[p][j] == 0)
			if (++p == n)
				return 0;
		if (p > j) {
			int64_t *r = m[j];
			m[j] = m[p];
			m[p] = r;
		}

		for (size_t i = j + 1; i < n; i++) {
			for (size_t k = j + 1; k < n; k++) {
				m[i][k] = m[j][j] * m[i][k] - m[i][j] * m[j][k];
				if (j == 0)
					continue;
				m[i][k] /= m[j-1][j-1];
			}
			m[i][j] = 0;
		}

	}

	return n;
}

