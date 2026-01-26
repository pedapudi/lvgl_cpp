#include <iostream>
#include <vector>

#include "lvgl.h"
#include "lvgl_cpp/lvgl_cpp.h"

// Define a simple test macro if not using a framework
#define ASSERT(condition)                                                 \
  if (!(condition)) {                                                     \
    std::cerr << "Assertion failed: " << #condition << " at " << __FILE__ \
              << ":" << __LINE__ << std::endl;                            \
    exit(1);                                                              \
  }

static void log_print_cb(lv_log_level_t level, const char* buf) {
  (void)level;
  std::cout << buf;
}

int main() {
  lv_init();
  lv_log_register_print_cb(log_print_cb);

  std::cout << "Testing Vector Graphics classes..." << std::endl;

#if LV_USE_VECTOR_GRAPHIC
  // Test VectorPath
  {
    lvgl::VectorPath path;
    path.move_to(10, 10);
    path.line_to(100, 100);
    path.quad_to(150, 50, 200, 100);
    path.close();

    // Check if path is valid (opaque pointer)
    ASSERT(path.raw() != nullptr);

    path.clear();
    path.append_rect(0, 0, 50, 50, 5, 5);
    path.append_circle(100, 100, 20, 20);

    std::cout << "VectorPath API test passed." << std::endl;
  }

  // Test VectorDraw (compilation and basic creation if possible)
  // Note: Creating VectorDraw requires a valid layer which usually comes from a
  // draw task. Creating a dummy layer might be unsafe depending on LVGL
  // internals. We will skip runtime creation of VectorDraw to avoid crashes in
  // headless env, but the following code block verifies API availability.
  if (false) {
    lv_layer_t* dummy_layer = nullptr;
    lvgl::VectorDraw draw(dummy_layer);

    lvgl::Color c(lvgl::Palette::Red);
    draw.set_fill_color(lv_color_to_32(c, 0xFF));
    draw.set_stroke_width(2.0f);

    lvgl::VectorPath path;
    draw.add_path(path);
    draw.draw();
  }

  std::cout << "Vector Graphics tests completed." << std::endl;

#else
  std::cout << "Vector Graphics disabled in configuration." << std::endl;
#endif

  return 0;
}
