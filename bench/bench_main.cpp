/*
 * Single-binary Benchmark Suite Runner
 */
#include <sys/resource.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "bench.h"
#include "lvgl.h"

#ifdef ENABLE_PROFILING
#include <gperftools/malloc_extension.h>
#endif

// Dummy flush callback for the display
static void flush_cb(lv_display_t* disp, const lv_area_t* area,
                     uint8_t* px_map) {
  lv_display_flush_ready(disp);
}

// Minimal LVGL environment setup
static void setup_lvgl() {
  lv_init();
  lv_display_t* disp = lv_display_create(800, 600);
  lv_display_set_flush_cb(disp, flush_cb);
  static uint8_t buf[800 * 10 * 4];  // Small partial buffer
  lv_display_set_buffers(disp, buf, nullptr, sizeof(buf),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);
}

int main(int argc, char** argv) {
  std::string mode = "";
  std::string arg = "";

  if (argc > 1) {
    std::string arg1 = argv[1];
    if (arg1 == "--list") {
      mode = "list";
    } else if (arg1.rfind("--run=", 0) == 0) {
      mode = "run";
      arg = arg1.substr(6);
    }
  }

  if (mode == "list") {
    std::cout << "[\n";
    const auto& benches = lvgl::bench::Registry::get().get_benchmarks();
    for (size_t i = 0; i < benches.size(); ++i) {
      std::cout << "  \"" << benches[i]->name() << "\"";
      if (i < benches.size() - 1) std::cout << ",";
      std::cout << "\n";
    }
    std::cout << "]\n";
    return 0;
  }

  if (mode == "run") {
    setup_lvgl();

    const auto& benches = lvgl::bench::Registry::get().get_benchmarks();
    lvgl::bench::Benchmark* target = nullptr;
    for (const auto& b : benches) {
      if (b->name() == arg) {
        target = b.get();
        break;
      }
    }

    if (!target) {
      std::cerr << "Benchmark not found: " << arg << "\n";
      return 1;
    }

    // Metrics recording
    struct rusage usage_start, usage_end;

    // Capture BASELINE heap before benchmark
    size_t heap_before = 0;
#ifdef ENABLE_PROFILING
    MallocExtension::instance()->GetNumericProperty(
        "generic.current_allocated_bytes", &heap_before);
#endif

    getrusage(RUSAGE_SELF, &usage_start);
    long rss_before = usage_start.ru_maxrss;

    auto start_time = std::chrono::high_resolution_clock::now();

    // Execute Benchmark
    lvgl::bench::State state;
    target->run(state);

    // Finalize any pending LVGL tasks
    lv_timer_handler();

    auto end_time = std::chrono::high_resolution_clock::now();
    getrusage(RUSAGE_SELF, &usage_end);

    long duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                           end_time - start_time)
                           .count();
    long rss_after = usage_end.ru_maxrss;
    long rss_delta_kb = rss_after - rss_before;

    // Capture FINAL heap after benchmark
    size_t heap_after = 0;
#ifdef ENABLE_PROFILING
    MallocExtension::instance()->GetNumericProperty(
        "generic.current_allocated_bytes", &heap_after);
#endif
    size_t heap_delta = heap_after - heap_before;

    // JSON Output
    std::cout << "{\n";
    std::cout << "  \"benchmark\": \"" << target->name() << "\",\n";
    std::cout << "  \"metrics\": {\n";
    std::cout << "    \"time_ns\": " << duration_ns << ",\n";
    std::cout << "    \"rss_kb\": " << rss_after << ",\n";
    std::cout << "    \"rss_delta_kb\": " << rss_delta_kb << ",\n";
    std::cout << "    \"heap_bytes\": " << heap_after << ",\n";
    std::cout << "    \"heap_delta_bytes\": " << heap_delta << "\n";
    std::cout << "  }\n";
    std::cout << "}\n";

    return 0;
  }

  std::cerr << "Usage: bench_suite --list | --run=<BenchmarkName>\n";
  return 1;
}
