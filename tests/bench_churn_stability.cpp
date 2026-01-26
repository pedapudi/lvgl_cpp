/*
 * Benchmark: Churn Stability (Issue #80)
 * Objective: Detect slow leaks by repeatedly creating and destroying a screen
 * over long duration. Metric: Monitors RSS over 1000 iterations.
 */

#include <sys/resource.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#include "lvgl.h"
#include "lvgl_cpp/core/object.h"
#include "lvgl_cpp/display/display.h"
#include "lvgl_cpp/widgets/button.h"
#include "lvgl_cpp/widgets/screen.h"

#define ITERATIONS 1000
#define WIDGETS_PER_SCREEN 20
#define REPORT_INTERVAL 50

void run_cycle() {
  auto screen = std::make_unique<lvgl::Screen>();

  std::vector<std::unique_ptr<lvgl::Button>> buttons;
  for (int i = 0; i < WIDGETS_PER_SCREEN; i++) {
    auto btn = std::make_unique<lvgl::Button>(screen.get());
    btn->set_pos(i * 10, i * 10);
    // Add event callback (potential leak source if not cleaned)
    btn->add_event_cb(LV_EVENT_CLICKED, [](lvgl::Event&) {});
    buttons.push_back(std::move(btn));
  }

  lv_timer_handler();
}

long get_rss_kb() {
  struct rusage usage;
  getrusage(RUSAGE_SELF, &usage);
  return usage.ru_maxrss;
}

int main(void) {
  lv_init();

  auto disp = lvgl::Display::create(800, 600);
  disp.set_flush_cb([](lvgl::Display* d, const lv_area_t* area,
                       uint8_t* px_map) { lv_display_flush_ready(d->raw()); });

  static uint8_t buf[800 * 10 * 4];
  disp.set_buffers(buf, nullptr, sizeof(buf), LV_DISPLAY_RENDER_MODE_PARTIAL);

  std::cout << "Starting Stability benchmark (" << ITERATIONS << " cycles)..."
            << std::endl;

  for (int i = 0; i <= ITERATIONS; i++) {
    run_cycle();

    if (i % REPORT_INTERVAL == 0) {
      long rss = get_rss_kb();
      std::cout << "METRIC_STABILITY: ITER=" << i << " RSS=" << rss
                << std::endl;
    }
  }

  std::cout << "Stability benchmark completed." << std::endl;
  return 0;
}
