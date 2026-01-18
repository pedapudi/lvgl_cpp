#pragma once

#include "lvgl.h"

namespace lvgl {

template <typename Derived>
class Sizable {
 public:
  Derived& set_width(int32_t value) {
    lv_obj_set_width(static_cast<Derived*>(this)->raw(), value);
    return *static_cast<Derived*>(this);
  }

  Derived& set_height(int32_t value) {
    lv_obj_set_height(static_cast<Derived*>(this)->raw(), value);
    return *static_cast<Derived*>(this);
  }

  Derived& set_size(int32_t width, int32_t height) {
    lv_obj_set_size(static_cast<Derived*>(this)->raw(), width, height);
    return *static_cast<Derived*>(this);
  }

  int32_t get_width() const {
    return lv_obj_get_width(static_cast<const Derived*>(this)->raw());
  }

  int32_t get_height() const {
    return lv_obj_get_height(static_cast<const Derived*>(this)->raw());
  }
};

} // namespace lvgl
