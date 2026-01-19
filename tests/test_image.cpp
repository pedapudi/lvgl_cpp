#include <iostream>

#include "../core/object.h"
#include "../display/display.h"
#include "../draw/image_descriptor.h"
#include "../widgets/image.h"
#include "lvgl.h"

void test_image_path() {
  std::cout << "Testing Image Path..." << std::endl;
  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::Image img(screen);

  // Test with a symbol string (treated same as file path for copy/safety)
  std::string path = LV_SYMBOL_OK;
  img.set_src(path);

  // Verify we can get it back
  const void* src = img.get_src();
  if (src) {
    lv_image_src_t type = lv_image_src_get_type(src);
    if (type == LV_IMAGE_SRC_SYMBOL) {
      std::cout << "PASS: Image source set as symbol." << std::endl;
    } else {
      std::cerr << "FAIL: Image source type mismatch: " << type << std::endl;
      exit(1);
    }
  } else {
    std::cerr << "FAIL: get_src returned null." << std::endl;
    exit(1);
  }
}

void test_image_descriptor() {
  std::cout << "Testing Image Descriptor..." << std::endl;
  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::Image img(screen);

  // Create a dummy raw image
  uint32_t w = 10;
  uint32_t h = 10;
  std::vector<uint8_t> data(w * h * 4, 0xFF);  // White image

  lvgl::ImageDescriptor dsc(w, h, LV_COLOR_FORMAT_ARGB8888, data.data(),
                            data.size());

  img.set_src(dsc);

  const void* src = img.get_src();
  if (src == dsc.raw()) {
    std::cout << "PASS: Image source set as descriptor." << std::endl;
  } else {
    std::cerr << "FAIL: Image source mismatch." << std::endl;
    exit(1);
  }
}

int main() {
  lv_init();
  lvgl::Display display = lvgl::Display::create(800, 480);

  test_image_path();
  test_image_descriptor();

  return 0;
}
