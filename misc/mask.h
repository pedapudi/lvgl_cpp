#ifndef LVGL_CPP_MISC_MASK_H_
#define LVGL_CPP_MISC_MASK_H_

#include "lvgl.h"
#include "src/draw/lv_draw.h"
#include "src/draw/lv_draw_mask.h"
#define LV_USE_PRIVATE_API 1
#include "src/draw/lv_draw_mask_private.h"
#undef LV_USE_PRIVATE_API

namespace lvgl {

class Mask {
 public:
  // Currently only Rectangle Mask is exposed via public API in lv_draw_mask.h
  struct Rect {
    lv_draw_mask_rect_dsc_t dsc;

    Rect() { lv_draw_mask_rect_dsc_init(&dsc); }

    void apply(lv_layer_t* layer) {
      if (layer) {
        lv_draw_mask_rect(layer, &dsc);
      }
    }
  };
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_MASK_H_
