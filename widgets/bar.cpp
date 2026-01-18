#include "bar.h"

#include "../core/observer.h"

#if LV_USE_BAR

namespace lvgl {

Bar::Bar() : Widget(lv_bar_create(lv_screen_active()), Ownership::Managed) {}

Bar::Bar(Object& parent, Ownership ownership)
    : Widget(lv_bar_create(parent.raw()), ownership) {}

Bar::Bar(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Bar& Bar::set_value(int32_t value, lv_anim_enable_t anim) {
  if (obj_) lv_bar_set_value(obj_, value, anim);
  return *this;
}

Bar& Bar::set_start_value(int32_t value, lv_anim_enable_t anim) {
  if (obj_) lv_bar_set_start_value(obj_, value, anim);
  return *this;
}

Bar& Bar::set_range(int32_t min, int32_t max) {
  if (obj_) lv_bar_set_range(obj_, min, max);
  return *this;
}

Bar& Bar::set_min_value(int32_t min) {
  if (obj_) lv_bar_set_min_value(obj_, min);
  return *this;
}

Bar& Bar::set_max_value(int32_t max) {
  if (obj_) lv_bar_set_max_value(obj_, max);
  return *this;
}

Bar& Bar::set_mode(lv_bar_mode_t mode) {
  if (obj_) lv_bar_set_mode(obj_, mode);
  return *this;
}

Bar& Bar::set_orientation(lv_bar_orientation_t orientation) {
  if (obj_) lv_bar_set_orientation(obj_, orientation);
  return *this;
}

int32_t Bar::get_value() const { return obj_ ? lv_bar_get_value(obj_) : 0; }

int32_t Bar::get_start_value() const {
  return obj_ ? lv_bar_get_start_value(obj_) : 0;
}

int32_t Bar::get_min_value() const {
  return obj_ ? lv_bar_get_min_value(obj_) : 0;
}

int32_t Bar::get_max_value() const {
  return obj_ ? lv_bar_get_max_value(obj_) : 100;
}

lv_bar_mode_t Bar::get_mode() const {
  return obj_ ? lv_bar_get_mode(obj_) : LV_BAR_MODE_NORMAL;
}

lv_bar_orientation_t Bar::get_orientation() const {
  return obj_ ? lv_bar_get_orientation(obj_) : LV_BAR_ORIENTATION_AUTO;
}

bool Bar::is_symmetrical() const {
  return obj_ ? lv_bar_is_symmetrical(obj_) : false;
}

Observer Bar::bind_value(Subject& subject) {
  return Observer(lv_bar_bind_value(raw(), subject.raw()));
}

}  // namespace lvgl

#endif  // LV_USE_BAR
