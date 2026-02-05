#pragma once

#include "lvgl_cpp/lvgl_cpp.h"

class HelloWorld {
 public:
  static void load(lvgl::Display& display, const std::string& text,
                   lvgl::Color bg_color, lvgl::Color text_color,
                   bool show_earth = true);
};
