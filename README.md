# CPU Cache Benchmark

This program tests how your CPU's cache affects memory access performance.

## How to Run

```bash
gcc -O2 benchmark.c -o benchmark
./benchmark
```

On Windows, use `benchmark.exe` instead of `./benchmark`.

## What It Does

The program accesses array elements in two patterns:
- **Sequential**: Elements in order (0, 1, 2, 3...)
- **Random**: Elements in unpredictable order

It tests five array sizes, from small (fits in cache) to large (exceeds cache).

## What to Look For

- Small arrays: Both patterns perform similarly
- Large arrays: Random access becomes much slower
- The "Ratio" column shows how many times slower random access is

The performance difference comes from your CPU's cache. When you access memory sequentially, the CPU predicts what you'll need next and loads it in advance. Random access defeats this prediction.