/*
 * Benchmark: Fragmentation (Scenario E)
 * Objective: Measure heap fragmentation via malloc_info (Linux only) or
 * simulated fragmentation.
 */

#include <functional>
#include <iostream>
#include <random>
#include <vector>

#include "lvgl.h"

#define MAX_ALLOCS 10000
#define ITERATIONS 50

int main(void) {
  // Note: This benchmark doesn't necessarily depend on LVGL objects,
  // but tests the impact of small closure allocations similar to what
  // EventSource does.

  std::cout << "Starting Fragmentation benchmark..." << std::endl;

  std::vector<std::function<void()>> closures;
  closures.reserve(MAX_ALLOCS);

  std::mt19937 rng(42);
  // Simulate closure sizes: 32 bytes to 128 bytes
  std::uniform_int_distribution<int> dist_op(0,
                                             2);  // 0: alloc, 1: free, 2: no-op

  // Capture simulation
  struct LargeCapture {
    char pad[64];
  };
  struct SmallCapture {
    char pad[16];
  };

  for (int i = 0; i < ITERATIONS; i++) {
    for (int j = 0; j < 100; j++) {
      int op = dist_op(rng);
      if (op == 0 && closures.size() < MAX_ALLOCS) {
        // Alloc
        if (rng() % 2 == 0) {
          SmallCapture c;
          closures.push_back([c]() { (void)c; });
        } else {
          LargeCapture c;
          closures.push_back([c]() { (void)c; });
        }
      } else if (op == 1 && !closures.empty()) {
        // Random free to create holes
        size_t idx = rng() % closures.size();
        closures[idx] = closures.back();
        closures.pop_back();
      }
    }
  }

  // In a real profiler run, we would trigger a heap dump here to check for
  // holes. For now, we simulate the workload.

  std::cout << "Fragmentation workload completed. Existing closures: "
            << closures.size() << std::endl;
  return 0;
}
