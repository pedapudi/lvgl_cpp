#include "image_button.h"

#if LV_USE_IMAGEBUTTON

namespace lvgl {

ImageButton::ImageButton() : ImageButton((Object*)nullptr) {}

ImageButton::ImageButton(Object* parent, Ownership ownership)
    : Object(lv_imagebutton_create(parent ? parent->raw() : nullptr),
             ownership) {}

ImageButton::ImageButton(lv_obj_t* obj, Ownership ownership)
    : Object(obj, ownership) {}

ImageButton& ImageButton::set_src(lv_imagebutton_state_t state,
                                  const void* src_left, const void* src_mid,
                                  const void* src_right) {
  if (obj_) lv_imagebutton_set_src(obj_, state, src_left, src_mid, src_right);
  return *this;
}

ImageButton& ImageButton::set_state(lv_imagebutton_state_t state) {
  if (obj_) lv_imagebutton_set_state(obj_, state);
  return *this;
}

ImageButton& ImageButton::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
ImageButton& ImageButton::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
ImageButton& ImageButton::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
ImageButton& ImageButton::align(lv_align_t align, int32_t x_ofs,
                                int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
ImageButton& ImageButton::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
ImageButton& ImageButton::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
ImageButton& ImageButton::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
ImageButton& ImageButton::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

const void* ImageButton::get_src_left(lv_imagebutton_state_t state) {
  return obj_ ? lv_imagebutton_get_src_left(obj_, state) : nullptr;
}

const void* ImageButton::get_src_middle(lv_imagebutton_state_t state) {
  return obj_ ? lv_imagebutton_get_src_middle(obj_, state) : nullptr;
}

const void* ImageButton::get_src_right(lv_imagebutton_state_t state) {
  return obj_ ? lv_imagebutton_get_src_right(obj_, state) : nullptr;
}

}  // namespace lvgl

#endif  // LV_USE_IMAGEBUTTON
