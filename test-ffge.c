/* -------------------------------------------------------------------------- *
 * test-ffge.c: test implementation of fraction-free Gaussian elimination.    *
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
#include <stdio.h>

#include <flint/fmpz.h>
#include <flint/fmpz_mat.h>

#include "ffge.h"
#include "xoshiro256ss.h"

static int TEST_RT = 0;
#define TEST_FAIL(...)	({						\
		fprintf(stderr, "FAIL %s:%d, %s() \"", 			\
			__FILE__, __LINE__, __func__);			\
		fprintf(stderr, __VA_ARGS__);				\
		fprintf(stderr, "\"\n");				\
		TEST_RT = -1;						\
	})

#define SEED UINT64_C(22349)
static struct xoshiro256ss RNG;

#define SIZE (3)
static int32_t MAT_ZER[SIZE * SIZE];
static int32_t MAT_ONE[SIZE * SIZE] = {
	1, 0, 2,
	1, 0, 2,
	1, 0, 2
};
static int32_t MAT_TWO[SIZE * SIZE] = {
	0, 0, 2,
	0, 0, 1,
	0, 1, 0
};
static int32_t MAT_THR[SIZE * SIZE] = {
	1, 0, 0,
	1, 2, 0,
	1, 3, 4
};


static void test_ffge_32i1_rank03(void)
{

	int32_t m[SIZE*SIZE];

#define TEST_MAT(MAT, ex_rt, ex_rnk)	({				\
		for (int i = 0; i < SIZE*SIZE; i++)			\
			m[i] = MAT[i];					\
		size_t rnk;						\
		int rt = ffge_32i1(SIZE, m, &rnk);			\
		if (rt != ex_rt || rnk != ex_rnk)			\
			TEST_FAIL("rank: %zu (ex: %d), rt: %d (ex: %d)",\
				rnk, ex_rnk, rt, ex_rt);		\
	})

	TEST_MAT(MAT_ZER, 0, 0);
	TEST_MAT(MAT_ONE, 0, 1);
	TEST_MAT(MAT_TWO, 0, 2);
	TEST_MAT(MAT_THR, 1, 3);
#undef TEST_MAT
}


/* Compute rank of 12x12 matrix with entries -1, 0, 1.
 * Compare result with flint.
 */
#define REPS	(9999)
#undef SIZE
#define SIZE	(12)
static int32_t A[SIZE * SIZE];


static void test_ffge_32i1_rank12(void)
{
	for (size_t r = 0; r < REPS; r++) {
		fmpz_mat_t B;
		fmpz_mat_init(B, SIZE, SIZE);
		for (size_t i = 0; i < SIZE; i++) {
			for (size_t j = 0; j < SIZE; j++) {
				int32_t x = (xoshiro256ss_next(&RNG) & 3) - 1;
				A[i*SIZE + j] = x;
				fmpz_set_si(fmpz_mat_entry(B, i, j), x);
			}
		}

		size_t rankA;
		int rt = ffge_32i1(SIZE, A, &rankA);
		int rankB = fmpz_mat_rank(B);

		if (rankA < SIZE && rt != 0)
			TEST_FAIL("rep. %zu, singular", r);
		if (rankA == SIZE && rt != 1)
			TEST_FAIL("rep. %zu, full-rank", r);

		if (rankA != (size_t)rankB)
			TEST_FAIL("rep. %zu, rankA: %zu, rankB: %zu",
					r, rankA, (size_t)rankB);

		fmpz_mat_clear(B);
	}
}


int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	xoshiro256ss_init(&RNG, SEED);

	test_ffge_32i1_rank03();
	test_ffge_32i1_rank12();

	return TEST_RT;
}
