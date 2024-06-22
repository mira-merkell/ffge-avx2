ffge-avx2 - Fraction Free Gaussian Elimination
==============================================

Implement a SIMD version of FFGE.  Benchmark agains FLINT.


Dependencies
------------

* libflint>=3.0 - [fast numerics library](https://flintlib.org).


How to use it
-------------

To run benchmarks:

```bash
make bench
```

Consult [Makefile](./Makefile) to set optimization flags to your liking.


References
----------

* George C. Nakos, Peter R. Turner, and Robert M. Williams. 1997.  Fraction-free
algorithms for linear and polynomial equations. SIGSAM Bull. 31, 3 (Sept.
1997), 11–19. [https://doi.org/10.1145/271130.271133](https://doi.org/10.1145/271130.271133)

* Turner, Peter R. A simplified fraction-free integer gauss elimination algorithm. Naval Air Warfare Center, Aircraft Division, 1995.

