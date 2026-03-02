#include "anim_image.h"

#if LV_USE_ANIMIMG

namespace lvgl {

AnimImage::AnimImage()
    : AnimImage(static_cast<Object*>(nullptr), Ownership::Managed) {}

AnimImage::AnimImage(Object* parent, Ownership ownership)
    : Widget(lv_animimg_create(parent ? parent->raw() : nullptr), ownership) {}

AnimImage::AnimImage(Object& parent) : AnimImage(&parent) {}

AnimImage::AnimImage(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

AnimImage& AnimImage::set_src(const void* dsc[], size_t num) {
  if (raw()) lv_animimg_set_src(raw(), dsc, num);
  return *this;
}

AnimImage& AnimImage::set_src_reverse(const void* dsc[], size_t num) {
  if (raw()) lv_animimg_set_src_reverse(raw(), dsc, num);
  return *this;
}

AnimImage& AnimImage::start() {
  if (raw()) lv_animimg_start(raw());
  return *this;
}

AnimImage& AnimImage::set_duration(uint32_t duration) {
  if (raw()) lv_animimg_set_duration(raw(), duration);
  return *this;
}

AnimImage& AnimImage::set_repeat_count(uint32_t count) {
  if (raw()) lv_animimg_set_repeat_count(raw(), count);
  return *this;
}

AnimImage& AnimImage::set_reverse_duration(uint32_t duration) {
  if (raw()) lv_animimg_set_reverse_duration(raw(), duration);
  return *this;
}

AnimImage& AnimImage::set_reverse_delay(uint32_t duration) {
  if (raw()) lv_animimg_set_reverse_delay(raw(), duration);
  return *this;
}

AnimImage& AnimImage::set_start_cb(lv_anim_start_cb_t cb) {
  if (raw()) lv_animimg_set_start_cb(raw(), cb);
  return *this;
}

AnimImage& AnimImage::set_completed_cb(lv_anim_completed_cb_t cb) {
  if (raw()) lv_animimg_set_completed_cb(raw(), cb);
  return *this;
}

const void** AnimImage::get_src() const {
  return raw() ? lv_animimg_get_src(raw()) : nullptr;
}

uint8_t AnimImage::get_src_count() const {
  return raw() ? lv_animimg_get_src_count(raw()) : 0;
}

uint32_t AnimImage::get_duration() const {
  return raw() ? lv_animimg_get_duration(raw()) : 0;
}

uint32_t AnimImage::get_repeat_count() const {
  return raw() ? lv_animimg_get_repeat_count(raw()) : 0;
}

lv_anim_t* AnimImage::get_anim() const {
  return raw() ? lv_animimg_get_anim(raw()) : nullptr;
}

}  // namespace lvgl

#endif  // LV_USE_ANIMIMG
