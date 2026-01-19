/*
 * Benchmark: Churn (Scenario D)
 * Objective: Detect slow leaks by repeatedly creating and destroying a screen.
 * Metric: Run indefinitely (or N iterations) without crash.
 */

#include <sys/resource.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "lvgl.h"
#include "lvgl_cpp/core/object.h"
#include "lvgl_cpp/widgets/button.h"
#include "lvgl_cpp/widgets/screen.h"

#define ITERATIONS 100
#define WIDGETS_PER_SCREEN 20

static void flush_cb(lv_display_t* disp, const lv_area_t* area,
                     uint8_t* px_map) {
  lv_display_flush_ready(disp);
}

void run_cycle() {
  // 1. Create a Screen (Wraps a new lv_obj_t)
  auto screen = std::make_unique<lvgl::Screen>();

  // 2. Add widgets
  std::vector<std::unique_ptr<lvgl::Button>> buttons;
  for (int i = 0; i < WIDGETS_PER_SCREEN; i++) {
    auto btn = std::make_unique<lvgl::Button>(screen.get());
    btn->set_pos(i * 10, i * 10);

    // Add callback to exercise EventSource leak path
    // Use lvgl::Event& signature!
    btn->add_event_cb([](lvgl::Event&) {}, LV_EVENT_CLICKED);

    buttons.push_back(std::move(btn));
  }

  lv_timer_handler();

  // 3. Destroy
}

int main(void) {
  lv_init();

  lv_display_t* disp = lv_display_create(800, 600);
  lv_display_set_flush_cb(disp, flush_cb);
  static uint8_t buf[800 * 10 * 4];
  lv_display_set_buffers(disp, buf, NULL, sizeof(buf),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);

  std::cout << "Starting Churn benchmark (" << ITERATIONS << " cycles)..."
            << std::endl;

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < ITERATIONS; i++) {
    if (i % 10 == 0) std::cout << "Cycle " << i << std::endl;
    run_cycle();
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto elapsed_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);

  std::cout << "BENCHMARK_METRIC: TIME=" << elapsed_ms << " unit=ms"
            << std::endl;
  std::cout << "BENCHMARK_METRIC: RSS=" << usage.ru_maxrss << " unit=kb"
            << std::endl;

  std::cout << "Churn benchmark completed." << std::endl;
  return 0;
}
