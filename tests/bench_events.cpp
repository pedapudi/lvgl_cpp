/*
 * Benchmark: Event Overhead (C++)
 * Objective: Measure cost of std::function + wrapper per callback.
 */

#include <sys/resource.h>

#include <chrono>
#include <iostream>
#include <vector>

#include "lvgl.h"
#include "lvgl_cpp/core/object.h"
#include "lvgl_cpp/widgets/button.h"

#define OBJ_COUNT 50

static void flush_cb(lv_display_t* disp, const lv_area_t* area,
                     uint8_t* px_map) {
  lv_display_flush_ready(disp);
}

int main(void) {
  lv_init();

  lv_display_t* disp = lv_display_create(800, 600);
  lv_display_set_flush_cb(disp, flush_cb);
  static uint8_t buf[800 * 10 * 4];
  lv_display_set_buffers(disp, buf, NULL, sizeof(buf),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);

  std::cout << "Starting C++ events benchmark (N=" << OBJ_COUNT << ")..."
            << std::endl;

  auto screen = lvgl::Object(lv_scr_act());
  std::vector<std::unique_ptr<lvgl::Button>> objects;
  objects.reserve(OBJ_COUNT);

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < OBJ_COUNT; i++) {
    auto btn = std::make_unique<lvgl::Button>(&screen);

    // Add lambda callback (capturing nothing to keep it small, but
    // std::function allocates anyway) Correct signature: lvgl::Event&
    btn->add_event_cb(
        [](lvgl::Event&) {
          // No-op
        },
        LV_EVENT_CLICKED);

    objects.push_back(std::move(btn));
  }

  lv_timer_handler();
  lv_timer_handler();

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

  return 0;
}
