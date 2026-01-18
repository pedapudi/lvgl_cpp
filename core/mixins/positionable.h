#pragma once

#include "../object.h"
#include "lvgl.h"

namespace lvgl {

template <typename Derived>
class Positionable {
 public:
  Derived& set_x(int32_t value) {
    lv_obj_set_x(static_cast<Derived*>(this)->raw(), value);
    return *static_cast<Derived*>(this);
  }

  Derived& set_y(int32_t value) {
    lv_obj_set_y(static_cast<Derived*>(this)->raw(), value);
    return *static_cast<Derived*>(this);
  }

  Derived& set_pos(int32_t x, int32_t y) {
    lv_obj_set_pos(static_cast<Derived*>(this)->raw(), x, y);
    return *static_cast<Derived*>(this);
  }

  Derived& align(Object::Align align, int32_t x_ofs = 0, int32_t y_ofs = 0) {
    lv_obj_set_align(static_cast<Derived*>(this)->raw(), static_cast<lv_align_t>(align));
    lv_obj_set_pos(static_cast<Derived*>(this)->raw(), x_ofs, y_ofs);
    return *static_cast<Derived*>(this);
  }
  
  Derived& align(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0) {
    lv_obj_set_align(static_cast<Derived*>(this)->raw(), align);
    lv_obj_set_pos(static_cast<Derived*>(this)->raw(), x_ofs, y_ofs);
    return *static_cast<Derived*>(this);
  }

  Derived& center() {
    lv_obj_center(static_cast<Derived*>(this)->raw());
    return *static_cast<Derived*>(this);
  }

  int32_t get_x() const {
    return lv_obj_get_x(static_cast<const Derived*>(this)->raw());
  }

  int32_t get_y() const {
    return lv_obj_get_y(static_cast<const Derived*>(this)->raw());
  }
};

} // namespace lvgl
