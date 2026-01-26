#include "theme.h"

namespace lvgl {

void Theme::set_current(Display* disp) {
  if (theme_) {
    lv_display_t* d = disp ? disp->raw() : lv_display_get_default();
    if (d) lv_display_set_theme(d, theme_);
  }
}

void Theme::apply(Object& obj) {
  if (theme_) {
    lv_theme_apply(obj.raw());
  }
}

Theme Theme::get_theme(Display* disp) {
  lv_display_t* d = disp ? disp->raw() : lv_display_get_default();
  return Theme(d ? lv_display_get_theme(d) : nullptr);
}

}  // namespace lvgl
