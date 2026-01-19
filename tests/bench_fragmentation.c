/*
 * Benchmark: Fragmentation (C Baseline)
 * Objective: Measure heap fragmentation using plain malloc/free.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <time.h>

#define MAX_ALLOCS 10000
#define ITERATIONS 50

// Simulate closure captures
typedef struct {
  char pad[16];
} small_capture_t;

typedef struct {
  char pad[64];
} large_capture_t;

// We store void pointers to simulated allocations
void* allocations[MAX_ALLOCS];
int alloc_count = 0;

int main(void) {
  printf("Starting Fragmentation C benchmark...\n");
  srand(42);

  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start);

  for (int i = 0; i < ITERATIONS; i++) {
    for (int j = 0; j < 100; j++) {
      int op = rand() % 3;  // 0: alloc, 1: free, 2: no-op

      if (op == 0 && alloc_count < MAX_ALLOCS) {
        // Alloc
        size_t size = (rand() % 2 == 0) ? sizeof(small_capture_t)
                                        : sizeof(large_capture_t);
        void* ptr = malloc(size);
        // Touch memory to ensure it's backed
        if (ptr) ((char*)ptr)[0] = 1;
        allocations[alloc_count++] = ptr;
      } else if (op == 1 && alloc_count > 0) {
        // Free random
        int idx = rand() % alloc_count;
        free(allocations[idx]);
        // Swap with last to keep array compact for tracking
        allocations[idx] = allocations[alloc_count - 1];
        alloc_count--;
      }
    }
  }

  clock_gettime(CLOCK_MONOTONIC, &end);
  double elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0 +
                      (end.tv_nsec - start.tv_nsec) / 1000000.0;

  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);

  printf("BENCHMARK_METRIC: TIME=%.2f unit=ms\n", elapsed_ms);
  printf("BENCHMARK_METRIC: RSS=%ld unit=kb\n", usage.ru_maxrss);

  printf("Fragmentation C workload completed. Existing allocations: %d\n",
         alloc_count);

  // Cleanup remaining
  for (int i = 0; i < alloc_count; i++) {
    free(allocations[i]);
  }

  return 0;
}
