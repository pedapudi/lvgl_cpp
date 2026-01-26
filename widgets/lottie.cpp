#include "lottie.h"

#if LV_USE_LOTTIE

namespace lvgl {

Lottie Lottie::create(Object& parent) {
  lv_obj_t* obj = lv_lottie_create(parent.raw());
  return Lottie(obj, Ownership::Managed);
}

void Lottie::set_src(const void* src, size_t src_size) {
  lv_lottie_set_src_data(raw(), src, src_size);
}

void Lottie::set_src(const char* path) { lv_lottie_set_src_file(raw(), path); }

void Lottie::set_buffer(int32_t w, int32_t h, void* buf) {
  lv_lottie_set_buffer(raw(), w, h, buf);
}

void Lottie::set_draw_buf(lv_draw_buf_t* draw_buf) {
  lv_lottie_set_draw_buf(raw(), draw_buf);
}

Animation Lottie::anim() {
  lv_anim_t* a = lv_lottie_get_anim(raw());
  // Animation wrapper doesn't own the lv_anim_t pointer here,
  // as it belongs to the Lottie widget text.
  // Ideally, Animation wrapper should handle non-owning pointers too.
  // Checking Animation ctor... Assumes explicit control.
  // For now, we return a wrapped animation. Be careful about lifetime.
  return Animation(a);
}

}  // namespace lvgl

#endif  // LV_USE_LOTTIE
