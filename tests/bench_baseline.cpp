/*
 * Benchmark: Baseline Object Creation (C++)
 * Objective: Measure fixed overhead of creating objects in C++ wrapper.
 */

#include <iostream>
#include <vector>

#include "lvgl.h"
#include "lvgl_cpp/core/object.h"
#include "lvgl_cpp/display/display.h"
#include "lvgl_cpp/widgets/button.h"

#define OBJ_COUNT 1000

// Mock driver
static void flush_cb(lv_display_t* disp, const lv_area_t* area,
                     uint8_t* px_map) {
  lv_display_flush_ready(disp);
}

int main(void) {
  lv_init();

  // C++ Display wrapper usage would be ideal, but for baseline equivalence
  // we stick to minimum C setup for the display to isolate widget costs.
  lv_display_t* disp = lv_display_create(800, 600);
  lv_display_set_flush_cb(disp, flush_cb);
  static uint8_t buf[800 * 10 * 4];
  lv_display_set_buffers(disp, buf, NULL, sizeof(buf),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);

  std::cout << "Starting C++ baseline benchmark (N=" << OBJ_COUNT << ")..."
            << std::endl;

  auto screen = lvgl::Object(lv_scr_act());  // Wrap active screen
  std::vector<std::unique_ptr<lvgl::Button>> objects;
  objects.reserve(OBJ_COUNT);

  for (int i = 0; i < OBJ_COUNT; i++) {
    // Create C++ wrapper button
    // Note: lvgl_cpp::Button by default creates a new C object in constructor.
    auto btn = std::make_unique<lvgl::Button>(&screen);
    btn->set_pos(i % 100, i / 100);
    btn->set_size(50, 30);
    objects.push_back(std::move(btn));
  }

  // Force a layout/update
  lv_timer_handler();

  std::cout << "Objects created. Sleeping..." << std::endl;

  return 0;
}
