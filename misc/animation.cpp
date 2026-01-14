#include "animation.h"

namespace lvgl {

void Animation::exec_cb_proxy(void* var, int32_t v) {
  // This is tricky. lv_anim_t struct is copied by lv_anim_start.
  // The 'var' is the object being animated.
  // We can't easily access the std::function from here unless we store it in
  // user_data of the animation. But lv_anim_start copies the struct, so
  // user_data is copied. If we store pointer to Animation object in user_data,
  // we can access it. BUT Animation object might be temporary (builder
  // pattern). So, for C++ callbacks, we probably need a persistent object or a
  // dynamic allocation that is managed. For now, let's only support C style
  // exec_cb (like lv_obj_set_x) or very specific wrappers. Implementing generic
  // C++ lambdas for animation exec_cb is hard without lifetime management. We
  // will implement what we can.
}

Animation::Animation() { lv_anim_init(&anim_); }

Animation::~Animation() {
  // nothing to clean up for stack-based anim struct
}

Animation& Animation::set_var(void* var) {
  lv_anim_set_var(&anim_, var);
  return *this;
}

Animation& Animation::set_exec_cb(lv_anim_exec_xcb_t exec_cb) {
  lv_anim_set_exec_cb(&anim_, exec_cb);
  return *this;
}

Animation& Animation::set_duration(uint32_t duration) {
  lv_anim_set_duration(&anim_, duration);
  return *this;
}

Animation& Animation::set_delay(uint32_t delay) {
  lv_anim_set_delay(&anim_, delay);
  return *this;
}

Animation& Animation::set_values(int32_t start, int32_t end) {
  lv_anim_set_values(&anim_, start, end);
  return *this;
}

Animation& Animation::set_path_cb(lv_anim_path_cb_t path_cb) {
  lv_anim_set_path_cb(&anim_, path_cb);
  return *this;
}

Animation& Animation::set_repeat_count(uint32_t cnt) {
  lv_anim_set_repeat_count(&anim_, cnt);
  return *this;
}

Animation& Animation::set_repeat_delay(uint32_t delay) {
  lv_anim_set_repeat_delay(&anim_, delay);
  return *this;
}

Animation& Animation::set_playback_duration(uint32_t duration) {
  lv_anim_set_reverse_duration(&anim_, duration);
  return *this;
}

Animation& Animation::set_playback_delay(uint32_t delay) {
  lv_anim_set_reverse_delay(&anim_, delay);
  return *this;
}

void Animation::start() { lv_anim_start(&anim_); }

}  // namespace lvgl
