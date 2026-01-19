/*
 * Benchmark: Fragmentation (C++ Wrapper)
 * Objective: Measure heap fragmentation via std::function closures.
 * Comparison: Should match bench_fragmentation.c allocation patterns.
 */

#include <sys/resource.h>

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

  struct timespec start_time;
  clock_gettime(CLOCK_MONOTONIC, &start_time);

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

  struct timespec end_time;
  clock_gettime(CLOCK_MONOTONIC, &end_time);
  double elapsed_ms = (end_time.tv_sec - start_time.tv_sec) * 1000.0 +
                      (end_time.tv_nsec - start_time.tv_nsec) / 1000000.0;

  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);

  std::cout << "BENCHMARK_METRIC: TIME=" << elapsed_ms << " unit=ms"
            << std::endl;
  std::cout << "BENCHMARK_METRIC: RSS=" << usage.ru_maxrss << " unit=kb"
            << std::endl;

  return 0;
}
