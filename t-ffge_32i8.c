#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

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

#define SEED	UINT64_C(93032)
static struct xoshiro256ss RNG;

#define REPS	UINT64_C(9999999)

int32_t ffge_32i8_modprim(uint64_t a);

int main(void)
{
	xoshiro256ss_init(&RNG, SEED);

	for (size_t i = 0; i < REPS; i++) {

		int64_t a = (int64_t)xoshiro256ss_next(&RNG);

		int32_t x, y;
		x = (int32_t)(a % FFGE_MAGPRIM);
		y = ffge_32i8_modprim(a);
		if (x != y)
			TEST_FAIL("rep. %zu, x=%u, y=%u", i, x, y);

	}

	return TEST_RT;
}
