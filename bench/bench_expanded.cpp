/*
 * Expanded Benchmarks for LVGL C++ Bindings
 * Covering Core Mechanisms, Layouts, and Complex Widgets
 */

#include <atomic>
#include <memory>
#include <string>
#include <vector>

#include "../core/group.h"
#include "../core/object.h"
#include "../core/observer.h"
#include "../misc/layout.h"
#include "../misc/style.h"
#include "../misc/timer.h"
#include "../widgets/label.h"
#include "../widgets/span.h"
#include "bench.h"
#include "../lvgl_cpp.h"

// --- 7.1 Core Mechanisms ---

// Observer Binding: Time to bind and trigger updates
LVGL_BENCHMARK(Core_Observer) {
  auto screen = std::make_unique<lvgl::Object>(lv_scr_act());
  lvgl::IntSubject subject(0);

  // Create listeners
  std::vector<std::unique_ptr<lvgl::Label>> labels;
  for (int i = 0; i < state.iterations; ++i) {
    auto label = std::make_unique<lvgl::Label>(screen.get());

    (void)subject.add_observer_obj(
        *label, [l = label.get(), &subject](lvgl::Observer*) {
          l->set_text_fmt("%d", subject.get());
        });
    labels.push_back(std::move(label));
  }

  // Trigger updates
  for (int i = 0; i < 100; ++i) {  // 100 updates per iteration count
    subject.set(i);
  }
}

// Style System: Creating and configuring styles
LVGL_BENCHMARK(Core_Style) {
  std::vector<lvgl::Style> styles;
  styles.reserve(state.iterations);

  for (int i = 0; i < state.iterations; ++i) {
    lvgl::Style s;
    s.set_bg_color(lv_color_hex(0xFF0000))
        .set_bg_opa(LV_OPA_50)
        .set_border_width(2)
        .set_border_color(lv_color_hex(0x00FF00))
        .set_radius(5)
        .set_text_color(lv_color_hex(0x0000FF))
        .set_pad_all(10)
        .set_shadow_width(5)
        .set_shadow_color(lv_color_black())
        .set_layout(LV_LAYOUT_FLEX);
    styles.push_back(std::move(s));
  }
}

// Timer System: Lifecycle cost
LVGL_BENCHMARK(Core_Timer) {
  std::vector<lvgl::Timer> timers;
  timers.reserve(state.iterations);

  std::atomic<int> counter{0};

  for (int i = 0; i < state.iterations; ++i) {
    lvgl::Timer t;
    t.set_period(100).set_cb([&counter](lvgl::Timer*) {
      counter.fetch_add(1, std::memory_order_relaxed);
    });
    timers.push_back(std::move(t));
  }
}

// Input Groups: Management overhead
LVGL_BENCHMARK(Core_Group) {
  auto group = std::make_unique<lvgl::Group>();
  auto screen = std::make_unique<lvgl::Object>(lv_scr_act());

  std::vector<std::unique_ptr<lvgl::Object>> objects;
  objects.reserve(state.iterations);

  for (int i = 0; i < state.iterations; ++i) {
    auto obj = std::make_unique<lvgl::Object>(screen.get());
    group->add_obj(*obj);
    objects.push_back(std::move(obj));
  }
}

// --- 7.3 Layout and Drawing ---

// Flex Layout: Layout recalculation cost
LVGL_BENCHMARK(Layout_Flex) {
  auto screen = std::make_unique<lvgl::Object>(lv_scr_act());
  auto container = std::make_unique<lvgl::Object>(screen.get());

  container->set_size(300, 300)
      .set_flex_flow(lvgl::FlexFlow::RowWrap)
      .set_flex_align(lvgl::FlexAlign::SpaceBetween, lvgl::FlexAlign::Center,
                      lvgl::FlexAlign::Center);

  std::vector<std::unique_ptr<lvgl::Object>> children;
  for (int i = 0; i < state.iterations; ++i) {
    auto obj = std::make_unique<lvgl::Object>(container.get());
    obj->set_size(20, 20);
    children.push_back(std::move(obj));
  }

  // Trigger reflow by changing container size
  container->set_width(200);
  lv_obj_update_layout(container->raw());

  container->set_width(400);
  lv_obj_update_layout(container->raw());
}

// --- 7.2 Complex Widgets (Extras) ---

// Rich Text: SpanGroup
LVGL_BENCHMARK(Widgets_Span) {
  auto screen = std::make_unique<lvgl::Object>(lv_scr_act());
  auto spangroup = std::make_unique<lvgl::SpanGroup>(screen.get());
  spangroup->set_size(300, 300);

  lvgl::Style red_style;
  red_style.set_text_color(lv_color_hex(0xFF0000));

  for (int i = 0; i < state.iterations; ++i) {
    lvgl::Span span = spangroup->add_span();
    span.set_text("Hello ").set_style(red_style);
  }

  // Force layout calc
  lv_obj_update_layout(spangroup->raw());
}

// --- 7.5 API Comparisons ---

// Fluent API overhead check
// Run: obj.set_size().set_pos() x Iterations
LVGL_BENCHMARK(API_Fluent) {
  auto screen = std::make_unique<lvgl::Object>(lv_scr_act());
  auto obj = std::make_unique<lvgl::Object>(screen.get());

  for (int i = 0; i < state.iterations * 10; ++i) {
    obj->set_size(i % 100, i % 100).set_pos(i % 200, i % 200).set_flex_grow(1);
  }
}
