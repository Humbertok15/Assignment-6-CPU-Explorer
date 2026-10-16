/*
 * CPU Cache Benchmark
 * Computer Science XII - Computer Systems
 * 
 * This program demonstrates how cache memory affects performance
 * by comparing sequential vs random memory access patterns.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ITERATIONS 100000000

// Sequential access: read array elements in order (0, 1, 2, 3...)
// This is cache-friendly because the CPU can predict what you need next
double test_sequential(int* array, int size) {
    clock_t start = clock();
    
    long sum = 0;
    for (int i = 0; i < ITERATIONS; i++) {
        sum += array[i % size];
    }
    
    clock_t end = clock();
    
    // Prevent compiler from optimizing away the sum
    if (sum == -999999) printf("never prints");
    
    return (double)(end - start) / CLOCKS_PER_SEC;
}

// Random access: read array elements in unpredictable order
// This is cache-unfriendly because the CPU can't predict what you need
double test_random(int* array, int* indices, int size) {
    clock_t start = clock();
    
    long sum = 0;
    for (int i = 0; i < ITERATIONS; i++) {
        sum += array[indices[i % size]];
    }
    
    clock_t end = clock();
    
    if (sum == -999999) printf("never prints");
    
    return (double)(end - start) / CLOCKS_PER_SEC;
}

// Shuffle array to create random access pattern
void shuffle(int* array, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

int main() {
    srand(42); // Fixed seed for reproducibility
    
    printf("==============================================\n");
    printf("CPU CACHE BENCHMARK\n");
    printf("==============================================\n");
    printf("Testing %d memory accesses per test\n\n", ITERATIONS);
    
    // Test different array sizes
    // Small arrays fit in L1 cache (~32KB = 8,000 ints)
    // Medium arrays fit in L2 cache (~256KB = 64,000 ints)
    // Large arrays fit in L3 cache (~8MB = 2,000,000 ints)
    // Huge arrays exceed L3 cache
    
    int sizes[] = {1000, 10000, 100000, 1000000, 10000000};
    char* labels[] = {"1K (fits in L1)", "10K (fits in L2)", "100K (fits in L3)", "1M (exceeds L3)", "10M (much larger than L3)"};
    int num_sizes = 5;
    
    printf("%-25s %15s %15s %10s\n", "Array Size", "Sequential", "Random", "Ratio");
    printf("----------------------------------------------------------------------\n");
    
    for (int t = 0; t < num_sizes; t++) {
        int size = sizes[t];
        
        // Allocate and initialize array
        int* array = (int*)malloc(size * sizeof(int));
        int* indices = (int*)malloc(size * sizeof(int));
        
        if (!array || !indices) {
            printf("Failed to allocate memory for size %d\n", size);
            continue;
        }
        
        for (int i = 0; i < size; i++) {
            array[i] = i;
            indices[i] = i;
        }
        shuffle(indices, size);
        
        // Run tests
        double seq_time = test_sequential(array, size);
        double rand_time = test_random(array, indices, size);
        double ratio = rand_time / seq_time;
        
        printf("%-25s %12.3f sec %12.3f sec %9.2fx\n", 
               labels[t], seq_time, rand_time, ratio);
        
        free(array);
        free(indices);
    }
    
    printf("\n==============================================\n");
    printf("WHAT THIS MEANS\n");
    printf("==============================================\n");
    printf("- Sequential access is faster because the CPU prefetches\n");
    printf("  nearby memory into cache before you ask for it.\n");
    printf("- Random access defeats prefetching - each access may\n");
    printf("  require fetching from slower memory levels.\n");
    printf("- As arrays grow beyond cache size, random access gets\n");
    printf("  much slower relative to sequential access.\n");
    printf("- The 'Ratio' column shows how many times slower random\n");
    printf("  access is compared to sequential access.\n");
    
    return 0;
}