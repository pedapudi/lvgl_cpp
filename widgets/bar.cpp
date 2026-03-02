#include "bar.h"

#include "../core/observer.h"

#if LV_USE_BAR

namespace lvgl {

Bar::Bar() : Bar(static_cast<Object*>(nullptr), Ownership::Managed) {}

Bar::Bar(Object* parent, Ownership ownership)
    : Widget(lv_bar_create(parent ? parent->raw() : nullptr), ownership) {}

Bar::Bar(Object& parent) : Bar(&parent) {}

Bar::Bar(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Bar& Bar::set_value(int32_t value, AnimEnable anim) {
#if LVGL_CPP_HAS_PROPERTIES
  if (anim == AnimEnable::Off)
    return set_property(LV_PROPERTY_BAR_VALUE, value);
#endif
  if (this->raw())
    lv_bar_set_value(this->raw(), value, static_cast<lv_anim_enable_t>(anim));
  return *this;
}

Bar& Bar::set_start_value(int32_t value, AnimEnable anim) {
#if LVGL_CPP_HAS_PROPERTIES
  if (anim == AnimEnable::Off)
    return set_property(LV_PROPERTY_BAR_START_VALUE, value);
#endif
  if (this->raw())
    lv_bar_set_start_value(this->raw(), value,
                           static_cast<lv_anim_enable_t>(anim));
  return *this;
}

Bar& Bar::set_range(int32_t min, int32_t max) {
  if (this->raw()) lv_bar_set_range(this->raw(), min, max);
  return *this;
}

Bar& Bar::set_min_value(int32_t min) {
#if LVGL_CPP_HAS_PROPERTIES
  return set_property(LV_PROPERTY_BAR_MIN_VALUE, min);
#else
  if (this->raw()) lv_bar_set_min_value(this->raw(), min);
  return *this;
#endif
}

Bar& Bar::set_max_value(int32_t max) {
#if LVGL_CPP_HAS_PROPERTIES
  return set_property(LV_PROPERTY_BAR_MAX_VALUE, max);
#else
  if (this->raw()) lv_bar_set_max_value(this->raw(), max);
  return *this;
#endif
}

Bar& Bar::set_mode(Mode mode) {
#if LVGL_CPP_HAS_PROPERTIES
  return set_property(LV_PROPERTY_BAR_MODE, static_cast<int32_t>(mode));
#else
  if (this->raw())
    lv_bar_set_mode(this->raw(), static_cast<lv_bar_mode_t>(mode));
  return *this;
#endif
}

Bar& Bar::set_orientation(Orientation orientation) {
#if LVGL_CPP_HAS_PROPERTIES
  return set_property(LV_PROPERTY_BAR_ORIENTATION,
                      static_cast<int32_t>(orientation));
#else
  if (this->raw())
    lv_bar_set_orientation(this->raw(),
                           static_cast<lv_bar_orientation_t>(orientation));
  return *this;
#endif
}

int32_t Bar::get_value() const {
  return this->raw() ? lv_bar_get_value(this->raw()) : 0;
}

int32_t Bar::get_start_value() const {
  return this->raw() ? lv_bar_get_start_value(this->raw()) : 0;
}

int32_t Bar::get_min_value() const {
  return this->raw() ? lv_bar_get_min_value(this->raw()) : 0;
}

int32_t Bar::get_max_value() const {
  return this->raw() ? lv_bar_get_max_value(this->raw()) : 100;
}

Bar::Mode Bar::get_mode() const {
  return this->raw() ? static_cast<Mode>(lv_bar_get_mode(this->raw()))
                     : Mode::Normal;
}

Bar::Orientation Bar::get_orientation() const {
  return this->raw()
             ? static_cast<Orientation>(lv_bar_get_orientation(this->raw()))
             : Orientation::Horizontal;
}

bool Bar::is_symmetrical() const {
  return this->raw() ? lv_bar_is_symmetrical(this->raw()) : false;
}

Observer Bar::bind_value(Subject& subject) {
  return Observer(lv_bar_bind_value(this->raw(), subject.raw()), false);
}

}  // namespace lvgl

#endif  // LV_USE_BAR
