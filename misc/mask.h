#ifndef LVGL_CPP_MISC_MASK_H_
#define LVGL_CPP_MISC_MASK_H_

#include "lvgl.h"

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
