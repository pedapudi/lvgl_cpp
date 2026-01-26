#include "lottie.h"

#if LV_USE_LOTTIE

namespace lvgl {

Lottie::Lottie() : Lottie(static_cast<Object*>(nullptr), Ownership::Managed) {}

Lottie::Lottie(Object* parent, Ownership ownership)
    : Widget(lv_lottie_create(parent ? parent->raw() : nullptr), ownership) {}

Lottie::Lottie(Object& parent) : Lottie(&parent) {}

Lottie::Lottie(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Lottie& Lottie::set_buffer(int32_t w, int32_t h, void* buf) {
  if (obj_) lv_lottie_set_buffer(obj_, w, h, buf);
  return *this;
}

Lottie& Lottie::set_draw_buf(lv_draw_buf_t* draw_buf) {
  if (obj_) lv_lottie_set_draw_buf(obj_, draw_buf);
  return *this;
}

Lottie& Lottie::set_src_data(const void* src, size_t src_size) {
  if (obj_) lv_lottie_set_src_data(obj_, src, src_size);
  return *this;
}

Lottie& Lottie::set_src_file(const char* src) {
  if (obj_) lv_lottie_set_src_file(obj_, src);
  return *this;
}

lv_anim_t* Lottie::get_anim() {
  return obj_ ? lv_lottie_get_anim(obj_) : nullptr;
}

}  // namespace lvgl

#endif  // LV_USE_LOTTIE
