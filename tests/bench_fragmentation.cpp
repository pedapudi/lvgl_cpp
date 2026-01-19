/*
 * Benchmark: Fragmentation (C++ Wrapper)
 * Objective: Measure heap fragmentation via std::function closures.
 * Comparison: Should match bench_fragmentation.c allocation patterns.
 */

#include <functional>
#include <iostream>
#include <random>
#include <vector>

#include "lvgl.h"

#define MAX_ALLOCS 10000
#define ITERATIONS 50

// Simulate closure captures matching C structs
struct SmallCapture {
  char pad[16];
};
struct LargeCapture {
  char pad[64];
};

int main(void) {
  std::cout << "Starting Fragmentation C++ benchmark..." << std::endl;

  // Vector to hold the closures (keeping them alive)
  std::vector<std::function<void()>> closures;
  closures.reserve(MAX_ALLOCS);

  std::mt19937 rng(42);
  std::uniform_int_distribution<int> dist_op(0,
                                             2);  // 0: alloc, 1: free, 2: no-op

  for (int i = 0; i < ITERATIONS; i++) {
    for (int j = 0; j < 100; j++) {
      int op = dist_op(rng);

      if (op == 0 && closures.size() < MAX_ALLOCS) {
        // Alloc
        if (rng() % 2 == 0) {
          SmallCapture c;
          // Capture by value copies the struct into the closure
          closures.push_back([c]() { (void)c; });
        } else {
          LargeCapture c;
          closures.push_back([c]() { (void)c; });
        }
      } else if (op == 1 && !closures.empty()) {
        // Random free
        size_t idx = rng() % closures.size();

        // Swap with last and pop to avoid shifting vector capability overhead
        // simulating random free in heap
        closures[idx] = closures.back();
        closures.pop_back();
      }
    }
  }

  std::cout << "Fragmentation C++ workload completed. Existing closures: "
            << closures.size() << std::endl;
  return 0;
}
