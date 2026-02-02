#include <memory>
#include <vector>

#include "bench.h"
#include "../lvgl_cpp.h"
#include "lvgl_cpp/core/object.h"
#include "lvgl_cpp/widgets/arc.h"
#include "lvgl_cpp/widgets/chart.h"
#include "lvgl_cpp/widgets/checkbox.h"
#include "lvgl_cpp/widgets/slider.h"
#include "lvgl_cpp/widgets/switch.h"
#include "lvgl_cpp/widgets/table.h"
#include "lvgl_cpp/widgets/textarea.h"

// Helper for keeping objects alive
template <typename T>
void run_widget_bench(lvgl::bench::State& state) {
  // Use specific count or state.iterations
  int count = state.iterations;
  // Override for heavy widgets if needed? Default is 100 in State.
  // The original bench used 50. Let's stick to state.iterations (100).

  auto screen = std::make_unique<lvgl::Object>(lv_scr_act());
  std::vector<std::unique_ptr<lvgl::Object>> objects;
  objects.reserve(count);

  for (int i = 0; i < count; i++) {
    auto obj = std::make_unique<T>(screen.get());
    lv_obj_set_pos(obj->raw(), i % 100, i / 100);
    objects.push_back(std::move(obj));
  }

  // Objects stay alive until function return
}

LVGL_BENCHMARK(Widgets_Slider) { run_widget_bench<lvgl::Slider>(state); }

LVGL_BENCHMARK(Widgets_Arc) { run_widget_bench<lvgl::Arc>(state); }

LVGL_BENCHMARK(Widgets_Switch) { run_widget_bench<lvgl::Switch>(state); }

LVGL_BENCHMARK(Widgets_Checkbox) {
  // Checkbox needs text setting to match old bench
  int count = state.iterations;
  auto screen = std::make_unique<lvgl::Object>(lv_scr_act());
  std::vector<std::unique_ptr<lvgl::Object>> objects;
  objects.reserve(count);

  for (int i = 0; i < count; i++) {
    auto cb = std::make_unique<lvgl::Checkbox>(screen.get());
    cb->set_text("Check me");
    lv_obj_set_pos(cb->raw(), i % 100, i / 100);
    objects.push_back(std::move(cb));
  }
}

LVGL_BENCHMARK(Widgets_Table) {
  int count = state.iterations;
  auto screen = std::make_unique<lvgl::Object>(lv_scr_act());
  std::vector<std::unique_ptr<lvgl::Object>> objects;
  objects.reserve(count);

  for (int i = 0; i < count; i++) {
    auto table = std::make_unique<lvgl::Table>(screen.get());
    table->set_row_count(5).set_column_count(3);
    table->cell(0, 0).set_value("Header1");
    table->cell(1, 1).set_value("Data");
    lv_obj_set_pos(table->raw(), i % 100, i / 100);
    objects.push_back(std::move(table));
  }
}

LVGL_BENCHMARK(Widgets_Chart) {
  int count = state.iterations;
  auto screen = std::make_unique<lvgl::Object>(lv_scr_act());
  std::vector<std::unique_ptr<lvgl::Object>> objects;
  objects.reserve(count);

  for (int i = 0; i < count; i++) {
    auto chart = std::make_unique<lvgl::Chart>(screen.get());
    chart->set_type(lvgl::Chart::Type::Line);
    chart->set_point_count(20);
    auto s1 =
        chart->add_series(lv_color_hex(0xFF0000), lvgl::Chart::Axis::PrimaryY);
    for (int j = 0; j < 20; j++) s1.set_next_value(j * 5);

    lv_obj_set_pos(chart->raw(), i % 100, i / 100);
    objects.push_back(std::move(chart));
  }
}
