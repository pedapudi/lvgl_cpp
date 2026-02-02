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
#include "lvgl_cpp/display/display.h"
#include "lvgl_cpp/widgets/button.h"
#include "lvgl_cpp/widgets/screen.h"

// ...

#define ITERATIONS 100
#define WIDGETS_PER_SCREEN 20

void run_cycle() {
  // 1. Create a Screen (Wraps a new lv_obj_t)
  auto screen = std::make_unique<lvgl::Screen>();

  // 2. Add widgets
  std::vector<std::unique_ptr<lvgl::Button>> buttons;
  for (int i = 0; i < WIDGETS_PER_SCREEN; i++) {
    auto btn = std::make_unique<lvgl::Button>(screen.get());
    btn->set_pos(i * 10, i * 10);

    // Add callback to exercise EventSource leak path
    btn->add_event_cb(lvgl::EventCode::Clicked, [](lvgl::Event&) {});

    buttons.push_back(std::move(btn));
  }

  lv_timer_handler();
  // 3. Destroy
}

int main(int argc, char** argv) {
  lv_init();

  // Use C++ Display Wrapper
  auto disp = lvgl::Display::create(800, 600);
  disp.set_flush_cb([](lvgl::Display* d, const lv_area_t* area,
                       uint8_t* px_map) { lv_display_flush_ready(d->raw()); });

  static uint8_t buf[800 * 10 * 4];
  disp.set_buffers(buf, nullptr, sizeof(buf), lvgl::Display::RenderMode::Partial);

  int duration_sec = 0;
  if (argc > 1) {
    std::string arg = argv[1];
    if (arg == "--duration" && argc > 2) {
      duration_sec = std::stoi(argv[2]);
    }
  }

  if (duration_sec > 0) {
    std::cout << "Starting Churn benchmark for " << duration_sec
              << " seconds..." << std::endl;
  } else {
    std::cout << "Starting Churn benchmark (" << ITERATIONS << " cycles)..."
              << std::endl;
  }

  auto start = std::chrono::high_resolution_clock::now();
  int cycles = 0;

  while (true) {
    if (cycles % 10 == 0) std::cout << "Cycle " << cycles << std::endl;
    run_cycle();
    cycles++;

    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::seconds>(now - start).count();

    if (duration_sec > 0) {
      if (elapsed >= duration_sec) break;
    } else {
      if (cycles >= ITERATIONS) break;
    }
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
