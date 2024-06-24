ffge-avx2 🦋
============

Fraction-free Gaussian elimination (FFGE).

* Implement a SIMD version of FFGE on a finite field: `Z mod 2^31 - 1`.
* Benchmark against FLINT.

This repository is WIP 🚧 The vectorized version of FFGE is incomplete.

Dependencies
------------

* CPU with AVX2 instruction set extensions
* C11 toolchain
* [NASM](https://nasm.us) assembler
* [FLINT >= 3.0](https://flintlib.org) and [GMP](https://gmplib.org/),
  to run benchmarks and tests


How to use it
-------------

Compile and test:

```bash
make && make test
```

To run benchmarks:

```bash
make clean && make bench
```

Consult [Makefile](./Makefile) how to set optimization flags to your liking.


Benchmarks
----------

Results:

### `bench-fullrank`

* Compute rank of a 12x12 matrix with elements from `{-1, 0, 1}`.
* Intel Core i9-12900, 64GB RAM, Ubuntu 22.04, GCC-14.1

```text
size: 12, reps: 65536
	fmpz_mat_rank(A)   2.011 μs
	ffge_64i1(B)       0.603 μs
	ffge_64i8(C)       0.316 μs
bench-fullrank: OK
```


References
----------

* George C. Nakos, Peter R. Turner, and Robert M. Williams. 1997.  Fraction-free
algorithms for linear and polynomial equations. SIGSAM Bull. 31, 3 (Sept.
1997), 11–19. [https://doi.org/10.1145/271130.271133](https://doi.org/10.1145/271130.271133)

* Turner, Peter R. A simplified fraction-free integer gauss elimination algorithm. Naval Air Warfare Center, Aircraft Division, 1995.

