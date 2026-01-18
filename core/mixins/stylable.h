#pragma once

#include "lvgl.h"

namespace lvgl {

template <typename Derived>
class Stylable {
 public:
  Derived& add_style(const lv_style_t* style, lv_style_selector_t selector = 0) {
    lv_obj_add_style(static_cast<Derived*>(this)->raw(), style, selector);
    return *static_cast<Derived*>(this);
  }

  Derived& remove_style(const lv_style_t* style, lv_style_selector_t selector = 0) {
    lv_obj_remove_style(static_cast<Derived*>(this)->raw(), style, selector);
    return *static_cast<Derived*>(this);
  }

  Derived& remove_all_styles() {
    lv_obj_remove_style_all(static_cast<Derived*>(this)->raw());
    return *static_cast<Derived*>(this);
  }
};

} // namespace lvgl
