#include <lvgl.h>

#include <iostream>

#include "lvgl_cpp/display/display.h"
#include "lvgl_cpp/widgets/gltf.h"

using namespace lvgl;

int main() {
  lv_init();
  lvgl::Display display = lvgl::Display::create(800, 480);
#if LV_USE_GLTF
  Gltf model;
  model.set_zoom(256);
  std::cout << "GLTF test passed!\n";
#else
  std::cout << "SKIP: GLTF not supported in this LVGL version\n";
#endif
  return 0;
}
