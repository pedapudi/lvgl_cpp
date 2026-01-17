#include "anim_image.h"

#if LV_USE_ANIMIMG

namespace lvgl {

AnimImage::AnimImage() : AnimImage((Object*)nullptr) {}

AnimImage::AnimImage(Object* parent, Ownership ownership)
    : Image(lv_animimg_create(parent ? parent->raw() : nullptr)) {}

AnimImage::AnimImage(lv_obj_t* obj, Ownership ownership) : Image(obj) {}

AnimImage& AnimImage::set_src(const void* dsc[], size_t num) {
  if (obj_) lv_animimg_set_src(obj_, dsc, num);
  return *this;
}

AnimImage& AnimImage::start() {
  if (obj_) lv_animimg_start(obj_);
  return *this;
}

AnimImage& AnimImage::set_duration(uint32_t duration) {
  if (obj_) lv_animimg_set_duration(obj_, duration);
  return *this;
}

AnimImage& AnimImage::set_repeat_count(uint32_t count) {
  if (obj_) lv_animimg_set_repeat_count(obj_, count);
  return *this;
}

AnimImage& AnimImage::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
AnimImage& AnimImage::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
AnimImage& AnimImage::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
AnimImage& AnimImage::align(Align align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
AnimImage& AnimImage::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
AnimImage& AnimImage::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
AnimImage& AnimImage::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
AnimImage& AnimImage::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

}  // namespace lvgl

#endif  // LV_USE_ANIMIMG
