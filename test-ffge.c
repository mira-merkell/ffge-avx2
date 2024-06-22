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

#include <x86intrin.h>

#include "ffge.h"

static int TEST_RT = 0;
#define TEST_FAIL(...)	({						\
		fprintf(stderr, "FAIL %s:%d \"", __FILE__, __LINE__);	\
		fprintf(stderr, __VA_ARGS__);				\
		fprintf(stderr, "\"\n");				\
		TEST_RT = -1;						\
	})


#define SIZE (3)
static int32_t MAT_ZERO[SIZE * SIZE];
static int32_t MAT_ONE[SIZE * SIZE] = {
	1, 0, 0,
	0, 1, 0,
	0, 0, 1
};


static void test_ffge_32i_rank01(void) {
	int rt;

	rt = ffge_32i(SIZE, MAT_ZERO);
	if (rt != -1)
		TEST_FAIL("matrix is singular");

	rt = ffge_32i(SIZE, MAT_ONE);
	if (rt != 0)
		TEST_FAIL("full-rank matrix");

}

int main(int argc, char **argv)
{
	(void)argc;
	(void)argv;

	test_ffge_32i_rank01();

	return TEST_RT;
}
