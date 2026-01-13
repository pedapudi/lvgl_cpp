#include "anim_image.h"

namespace lvgl {

AnimImage::AnimImage() : AnimImage((Object*)nullptr) {}

AnimImage::AnimImage(Object* parent)
    : Image(lv_animimg_create(parent ? parent->raw() : nullptr)) {}

AnimImage::AnimImage(lv_obj_t* obj) : Image(obj) {}

void AnimImage::set_src(const void* dsc[], size_t num) {
  if (obj_) lv_animimg_set_src(obj_, dsc, num);
}

void AnimImage::start() {
  if (obj_) lv_animimg_start(obj_);
}

void AnimImage::set_duration(uint32_t duration) {
  if (obj_) lv_animimg_set_duration(obj_, duration);
}

void AnimImage::set_repeat_count(uint32_t count) {
  if (obj_) lv_animimg_set_repeat_count(obj_, count);
}

}  // namespace lvgl
