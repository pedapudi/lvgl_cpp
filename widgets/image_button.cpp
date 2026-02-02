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

ImageButton& ImageButton::set_src(State state, const void* src_left,
                                  const void* src_mid, const void* src_right) {
  if (raw())
    lv_imagebutton_set_src(raw(), static_cast<lv_imagebutton_state_t>(state),
                           src_left, src_mid, src_right);
  return *this;
}

ImageButton& ImageButton::set_state(State state) {
  if (raw())
    lv_imagebutton_set_state(raw(), static_cast<lv_imagebutton_state_t>(state));
  return *this;
}

const void* ImageButton::get_src_left(State state) {
  return raw() ? lv_imagebutton_get_src_left(
                     raw(), static_cast<lv_imagebutton_state_t>(state))
               : nullptr;
}

const void* ImageButton::get_src_middle(State state) {
  return raw() ? lv_imagebutton_get_src_middle(
                     raw(), static_cast<lv_imagebutton_state_t>(state))
               : nullptr;
}

const void* ImageButton::get_src_right(State state) {
  return raw() ? lv_imagebutton_get_src_right(
                     raw(), static_cast<lv_imagebutton_state_t>(state))
               : nullptr;
}

}  // namespace lvgl

#endif  // LV_USE_IMAGEBUTTON
