#include "image_button.h"

#if LV_USE_IMAGEBUTTON

namespace lvgl {

ImageButton::ImageButton()
    : ImageButton(static_cast<Object*>(nullptr), Ownership::Managed) {}

ImageButton::ImageButton(Object* parent, Ownership ownership)
    : Widget(lv_imagebutton_create(parent ? parent->raw() : nullptr),
             ownership) {}

ImageButton::ImageButton(Object& parent) : ImageButton(&parent) {}

ImageButton::ImageButton(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

ImageButton& ImageButton::set_src(lv_imagebutton_state_t state,
                                  const void* src_left, const void* src_mid,
                                  const void* src_right) {
  if (obj_) lv_imagebutton_set_src(obj_, state, src_left, src_mid, src_right);
  return *this;
}

ImageButton& ImageButton::set_src(State state, const void* src_left,
                                  const void* src_mid, const void* src_right) {
  if (obj_)
    lv_imagebutton_set_src(obj_, static_cast<lv_imagebutton_state_t>(state),
                           src_left, src_mid, src_right);
  return *this;
}

ImageButton& ImageButton::set_state(lv_imagebutton_state_t state) {
  if (obj_) lv_imagebutton_set_state(obj_, state);
  return *this;
}

ImageButton& ImageButton::set_state(State state) {
  if (obj_)
    lv_imagebutton_set_state(obj_, static_cast<lv_imagebutton_state_t>(state));
  return *this;
}

const void* ImageButton::get_src_left(lv_imagebutton_state_t state) {
  return obj_ ? lv_imagebutton_get_src_left(obj_, state) : nullptr;
}

const void* ImageButton::get_src_left(State state) {
  return get_src_left(static_cast<lv_imagebutton_state_t>(state));
}

const void* ImageButton::get_src_middle(lv_imagebutton_state_t state) {
  return obj_ ? lv_imagebutton_get_src_middle(obj_, state) : nullptr;
}

const void* ImageButton::get_src_middle(State state) {
  return get_src_middle(static_cast<lv_imagebutton_state_t>(state));
}

const void* ImageButton::get_src_right(lv_imagebutton_state_t state) {
  return obj_ ? lv_imagebutton_get_src_right(obj_, state) : nullptr;
}

const void* ImageButton::get_src_right(State state) {
  return get_src_right(static_cast<lv_imagebutton_state_t>(state));
}

}  // namespace lvgl

#endif  // LV_USE_IMAGEBUTTON
