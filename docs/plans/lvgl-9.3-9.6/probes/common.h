#include "lvgl_cpp.h"
#include "misc/style.h"
#include <cstdio>
using namespace lvgl;
static void init() {
  lv_init();
  static uint8_t buf[64*64*4];
  lv_display_t* d = lv_display_create(64, 64);
  lv_display_set_buffers(d, buf, nullptr, sizeof(buf), LV_DISPLAY_RENDER_MODE_PARTIAL);
  lv_display_set_flush_cb(d, [](lv_display_t* d, const lv_area_t*, uint8_t*) { lv_display_flush_ready(d); });
}
