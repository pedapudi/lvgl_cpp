#include <cassert>
#include <iostream>
#include <vector>

#include "../display/display.h"
#include "../draw/image_descriptor.h"
#include "../misc/style.h"
#include "../widgets/button.h"
#include "lvgl.h"

int main() {
  lv_init();
  lvgl::Display display = lvgl::Display::create(800, 480);

  std::cout << "Testing Issue 161: Image Source Standardization..."
            << std::endl;

  // 1. Create a dummy image data
  std::vector<uint8_t> img_data(100 * 100 * 4, 0xAA);
  lvgl::ImageDescriptor dsc(100, 100, lvgl::ColorFormat::ARGB8888, img_data.data(),
                            img_data.size());

  assert(dsc.is_valid());

  // 2. Test with Persistent Style
  lvgl::Style style;
  style.bg_image_src(dsc);

  lv_style_value_t v;
  lv_style_get_prop(style.raw(), LV_STYLE_BG_IMAGE_SRC, &v);
  const void* style_src = v.ptr;
  assert(style_src == dsc.raw());

  // 3. Test with Local Style (StyleProxy)
  lvgl::Button btn;
  btn.style().bg_image_src(dsc);

  const void* obj_src = lv_obj_get_style_bg_image_src(btn.raw(), LV_PART_MAIN);
  assert(obj_src == dsc.raw());

  // 4. Test Arc Image Source
  btn.style().arc_image_src(dsc);
  const void* arc_src = lv_obj_get_style_arc_image_src(btn.raw(), LV_PART_MAIN);
  assert(arc_src == dsc.raw());

  std::cout << "[SUCCESS] Image Source Standardization verified." << std::endl;
  return 0;
}
