/*
 * Benchmark: Fragmentation (C Baseline)
 * Objective: Measure heap fragmentation using plain malloc/free.
 */

#include <stdio.h>
#include <stdlib.h>
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

  printf("Fragmentation C workload completed. Existing allocations: %d\n",
         alloc_count);

  // Cleanup remaining
  for (int i = 0; i < alloc_count; i++) {
    free(allocations[i]);
  }

  return 0;
}
