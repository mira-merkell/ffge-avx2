ffge-avx2 - fraction-free Gaussian elimination
==============================================

Implement a SIMD version of FFGE on a finite field.
Benchmark against FLINT.

This repository is WIP 🚧. The vectorized version of FFGE is incomplete.

Dependencies
------------

* CPU supporting AVX2 instruction set extensions
* libflint>=3.0 - [fast numerics library](https://flintlib.org)


How to use it
-------------

To run benchmarks:

```bash
make clean && make bench
```

Consult [Makefile](./Makefile) how to set optimization flags to your liking.


Benchmarks
----------

Results:

### `bench-fullrank`

* Check if a matrix has full rank
* Intel Core i9-12900, 64GB RAM, Ubuntu 22.04, GCC-14.1

```text
size: 12, reps: 65536
	fmpz_mat_rank(A)   2.000 μs
	ffge_64i1(B)       0.615 μs
	ffge_64i4(C)       0.123 μs
bench-fullrank: OK
```


References
----------

* George C. Nakos, Peter R. Turner, and Robert M. Williams. 1997.  Fraction-free
algorithms for linear and polynomial equations. SIGSAM Bull. 31, 3 (Sept.
1997), 11–19. [https://doi.org/10.1145/271130.271133](https://doi.org/10.1145/271130.271133)

* Turner, Peter R. A simplified fraction-free integer gauss elimination algorithm. Naval Air Warfare Center, Aircraft Division, 1995.

