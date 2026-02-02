/*
 * Benchmark: Fragmentation (C++ Wrapper)
 * Objective: Measure heap fragmentation using actual LVGL widgets and C++
 * callbacks. Comparison: Matches bench_fragmentation.c allocation patterns.
 */

#include <sys/resource.h>
#include <unistd.h>

#include <functional>
#include <iostream>
#include <random>
#include <vector>

#include "../lvgl_cpp.h"
#include "lvgl_cpp/display/display.h"
#include "lvgl_cpp/lvgl_cpp.h"
#include "lvgl_cpp/widgets/button.h"

#define MAX_ALLOCS 1000
#define ITERATIONS 50

struct CaptureState {
  char pad[64];
};

int main(void) {
  lv_init();

  // Need a display for widgets (even if partial)
  lv_display_t* d = lv_display_create(800, 600);
  lvgl::Display display(d);
  lv_display_set_flush_cb(
      d, [](lv_display_t* d, const lv_area_t* area, uint8_t* px_map) {
        lv_display_flush_ready(d);
      });
  static uint8_t buf[800 * 10 * 4];
  display.set_buffers(buf, nullptr, sizeof(buf),
                      lvgl::Display::RenderMode::Partial);

  std::cout << "Starting Fragmentation C++ benchmark (LVGL Widgets)..."
            << std::endl;

  struct timespec start_time;
  clock_gettime(CLOCK_MONOTONIC, &start_time);

  // Keep track of created widgets
  std::vector<lvgl::Object> objects;
  objects.reserve(MAX_ALLOCS);

  // Root screen
  lvgl::Object screen(lv_screen_active());

  std::mt19937 rng(42);
  std::uniform_int_distribution<int> dist_op(0,
                                             2);  // 0: alloc, 1: free, 2: no-op

  for (int i = 0; i < ITERATIONS; i++) {
    for (int j = 0; j < 50; j++) {
      int op = dist_op(rng);

      if (op == 0 && objects.size() < MAX_ALLOCS) {
        // Alloc: Create Button + Attach Callback
        lvgl::Button btn(screen);

        // Add a lambda with capture to exercise std::function allocation
        CaptureState state;
        btn.add_event_cb(lvgl::EventCode::Clicked, [state](lvgl::Event& e) {
          (void)state;
          (void)e;
        });

        objects.push_back(std::move(btn));

      } else if (op == 1 && !objects.empty()) {
        // Free: Delete random object
        size_t idx = rng() % objects.size();

        // Explicitly delete the underlying LVGL object
        // The wrapper is stateless/view-like generally, but we want to ensure
        // the LVGL memory is freed.
        lv_obj_delete(objects[idx].raw());

        // Remove from vector
        objects[idx] = std::move(objects.back());
        objects.pop_back();
      }
    }
    // Simulate tick to allow LVGL to process/cleanup
    lv_timer_handler();
  }

  std::cout << "Fragmentation C++ workload completed. Objects alive: "
            << objects.size() << std::endl;

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
