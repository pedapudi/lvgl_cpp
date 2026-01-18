#include "anim_image.h"

#if LV_USE_ANIMIMG

namespace lvgl {

AnimImage::AnimImage()
    : Widget(lv_animimg_create(lv_screen_active()), Ownership::Managed) {}

AnimImage::AnimImage(Object& parent)
    : Widget(lv_animimg_create(parent.raw()), Ownership::Managed) {}

AnimImage::AnimImage(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

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

}  // namespace lvgl

#endif  // LV_USE_ANIMIMG
