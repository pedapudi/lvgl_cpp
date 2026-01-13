#include "slider.h"
#include "../core/observer.h"

namespace lvgl {

Slider::Slider() : Slider((Object *)nullptr) {}

Slider::Slider(Object *parent)
    : Bar(lv_slider_create(parent ? parent->raw() : nullptr)) {}

Slider::Slider(lv_obj_t *obj) : Bar(obj) {}

void Slider::set_left_value(int32_t value, lv_anim_enable_t anim) {
  if (obj_)
    lv_slider_set_start_value(obj_, value, anim);
}

int32_t Slider::get_left_value() const {
  return obj_ ? lv_slider_get_left_value(obj_) : 0;
}

bool Slider::is_dragged() const {
  return obj_ ? lv_slider_is_dragged(obj_) : false;
}

Observer Slider::bind_value(Subject &subject) {
  return Observer(lv_slider_bind_value(raw(), subject.raw()));
}

} // namespace lvgl
