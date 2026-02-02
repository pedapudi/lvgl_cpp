/*
 * Benchmark: Various Widgets (C++)
 * Objective: Measure creation overhead of specific widgets.
 * Usage: ./bench_widgets <WIDGET_TYPE>
 */

#include <sys/resource.h>

#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "lvgl.h"
#include "lvgl_cpp/core/object.h"
#include "lvgl_cpp/display/display.h"

// Include specific widgets
#include "lvgl_cpp/widgets/arc.h"
#include "lvgl_cpp/widgets/chart.h"
#include "lvgl_cpp/widgets/checkbox.h"
#include "lvgl_cpp/widgets/slider.h"
#include "lvgl_cpp/widgets/switch.h"
#include "lvgl_cpp/widgets/table.h"
#include "lvgl_cpp/widgets/textarea.h"

#define OBJ_COUNT 50

// Mock driver
static void flush_cb(lv_display_t* disp, const lv_area_t* area,
                     uint8_t* px_map) {
  lv_display_flush_ready(disp);
}

int main(int argc, char** argv) {
  lv_init();

  lv_display_t* d = lv_display_create(800, 600);
  lvgl::Display display(d);
  lv_display_set_flush_cb(d, flush_cb);
  static uint8_t buf[800 * 10 * 4];
  display.set_buffers(buf, nullptr, sizeof(buf),
                      lvgl::Display::RenderMode::Partial);

  std::string widget_type = "slider";  // default
  if (argc > 1) widget_type = argv[1];

  std::cout << "Starting C++ benchmark for " << widget_type
            << " (N=" << OBJ_COUNT << ")..." << std::endl;

  // We wrap the active screen but do not own it
  auto screen = std::make_unique<lvgl::Object>(lv_scr_act());
  std::vector<std::unique_ptr<lvgl::Object>> objects;
  objects.reserve(OBJ_COUNT);

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < OBJ_COUNT; i++) {
    std::unique_ptr<lvgl::Object> obj;

    if (widget_type == "arc") {
      obj = std::make_unique<lvgl::Arc>(screen.get());
    } else if (widget_type == "checkbox") {
      auto cb = std::make_unique<lvgl::Checkbox>(screen.get());
      cb->set_text("Check me");
      obj = std::move(cb);
    } else if (widget_type == "slider") {
      obj = std::make_unique<lvgl::Slider>(screen.get());
    } else if (widget_type == "switch") {
      obj = std::make_unique<lvgl::Switch>(screen.get());
    } else if (widget_type == "textarea") {
      auto ta = std::make_unique<lvgl::Textarea>(screen.get());
      ta->set_text("Hello");
      obj = std::move(ta);
    } else if (widget_type == "chart") {
      auto chart = std::make_unique<lvgl::Chart>(screen.get());
      chart->set_type(lvgl::Chart::Type::Line);
      chart->set_point_count(20);
      auto s1 = chart->add_series(lv_color_hex(0xFF0000),
                                  lvgl::Chart::Axis::PrimaryY);
      for (int j = 0; j < 20; j++) s1.set_next_value(j * 5);
      obj = std::move(chart);
    } else if (widget_type == "table") {
      auto table = std::make_unique<lvgl::Table>(screen.get());
      table->set_row_count(5).set_column_count(3);
      table->cell(0, 0).set_value("Header1");
      table->cell(1, 1).set_value("Data");
      obj = std::move(table);
    } else {
      std::cerr << "Unknown widget type: " << widget_type << std::endl;
      return 1;
    }

    // Use C API for positioning since generic Object wrapper doesn't expose it
    // Or use the newly verified fluent set_x/y if
    // Stylable/Positionable/Layoutable is fully integrated? bench_widgets.cpp
    // includes "lvgl_cpp/core/object.h" but might not have full Mixin
    // definitions visible unless Widget is fully defined. Let's stick to C API
    // for positioning to be safe.
    lv_obj_set_pos(obj->raw(), i % 100, i / 100);
    lv_obj_set_size(obj->raw(), 100, 100);

    objects.push_back(std::move(obj));
  }

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
