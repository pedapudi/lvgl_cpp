#include "animation.h"

#include "anim_exec_callback.h"
#include "anim_path_callback.h"

namespace lvgl {

AnimationHandle::AnimationHandle(void* var, lv_anim_exec_xcb_t exec_cb)
    : var_(var), exec_cb_(exec_cb) {}

bool AnimationHandle::is_running() const {
  if (!var_) return false;
  return lv_anim_get(var_, exec_cb_) != nullptr;
}

void AnimationHandle::stop() {
  if (var_) {
    lv_anim_delete(var_, exec_cb_);
  }
}

Animation::Animation() {
  ptr_ = &anim_;
  lv_anim_init(ptr_);
}

Animation::Animation(lv_anim_t* anim) : ptr_(anim) {}

Animation::~Animation() {
  // nothing to clean up for stack-based anim struct
  // user_data_ unique_ptr will handle itself
}

Animation::Animation(void* var, int32_t start_val, int32_t end_val,
                     uint32_t duration)
    : Animation() {
  set_var(var);
  set_values(start_val, end_val);
  set_duration(duration);
}

Animation::Animation(const Object& object) : Animation() { set_var(object); }

Animation& Animation::set_var(void* var) {
  lv_anim_set_var(ptr_, var);
  return *this;
}

Animation& Animation::set_var(const Object& object) {
  set_var(object.raw());
  return *this;
}

Animation& Animation::set_exec_cb(lv_anim_exec_xcb_t exec_cb) {
  lv_anim_set_exec_cb(ptr_, exec_cb);
  return *this;
}

Animation& Animation::set_duration(uint32_t duration) {
  lv_anim_set_duration(ptr_, duration);
  return *this;
}

Animation& Animation::set_duration(std::chrono::milliseconds duration) {
  return set_duration(static_cast<uint32_t>(duration.count()));
}

Animation& Animation::set_delay(uint32_t delay) {
  lv_anim_set_delay(ptr_, delay);
  return *this;
}

Animation& Animation::set_delay(std::chrono::milliseconds delay) {
  return set_delay(static_cast<uint32_t>(delay.count()));
}

Animation& Animation::set_values(int32_t start, int32_t end) {
  lv_anim_set_values(ptr_, start, end);
  return *this;
}

Animation& Animation::set_path_cb(lv_anim_path_cb_t path_cb) {
  lv_anim_set_path_cb(ptr_, path_cb);
  return *this;
}

Animation& Animation::set_repeat_count(uint32_t cnt) {
  lv_anim_set_repeat_count(ptr_, cnt);
  return *this;
}

Animation& Animation::set_repeat_delay(uint32_t delay) {
  lv_anim_set_repeat_delay(ptr_, delay);
  return *this;
}

Animation& Animation::set_playback_duration(uint32_t duration) {
  lv_anim_set_reverse_duration(ptr_, duration);
  return *this;
}

Animation& Animation::set_playback_delay(uint32_t delay) {
  lv_anim_set_reverse_delay(ptr_, delay);
  return *this;
}

void Animation::exec_cb_proxy(lv_anim_t* a, int32_t v) {
  CallbackData* data = static_cast<CallbackData*>(a->user_data);
  if (data && data->exec_cb) {
    data->exec_cb(a->var, v);
  }
}

int32_t Animation::path_cb_proxy(const lv_anim_t* a) {
  CallbackData* data = static_cast<CallbackData*>(a->user_data);
  if (data && data->path_cb) {
    return data->path_cb(a);
  }
  return lv_anim_path_linear(a);  // Fallback
}

void Animation::completed_cb_proxy(lv_anim_t* a) {
  CallbackData* data = static_cast<CallbackData*>(a->user_data);
  if (data && data->completed_cb) {
    data->completed_cb();
  }
}

void Animation::deleted_cb_proxy(lv_anim_t* a) {
  CallbackData* data = static_cast<CallbackData*>(a->user_data);
  if (data) {
    if (data->deleted_cb) {
      data->deleted_cb();
    }
    delete data;
  }
}

Animation& Animation::set_exec_cb(ExecCallback cb) {
  if (!user_data_) user_data_ = std::make_unique<CallbackData>();
  user_data_->exec_cb = std::move(cb);
  // We don't set user_data on anim_ yet, we do it at start() to allow multiple
  // instances
  return *this;
}

Animation& Animation::set_exec_cb(ObjectExecCallback cb) {
  // Trampoline to convert void* -> Object&
  auto trampoline = [cb](void* var, int32_t v) {
    if (var) {
      // Safety Note: This assumes var is an lv_obj_t*.
      // We check for null, but we can't fully verify type at runtime here.
      Object obj(static_cast<lv_obj_t*>(var), Object::Ownership::Unmanaged);
      cb(obj, v);
    }
  };
  return set_exec_cb(static_cast<ExecCallback>(trampoline));
}

Animation& Animation::set_path_cb(const PathCallback& cb) {
  if (!user_data_) user_data_ = std::make_unique<CallbackData>();
  user_data_->path_cb = cb;
  return *this;
}

Animation::Exec::Callback Animation::Exec::X() {
  return (lv_anim_exec_xcb_t)lv_obj_set_x;
}

Animation::Exec::Callback Animation::Exec::Y() {
  return (lv_anim_exec_xcb_t)lv_obj_set_y;
}

Animation::Exec::Callback Animation::Exec::Width() {
  return (lv_anim_exec_xcb_t)lv_obj_set_width;
}

Animation::Exec::Callback Animation::Exec::Height() {
  return (lv_anim_exec_xcb_t)lv_obj_set_height;
}

Animation::Exec::Callback Animation::Exec::Opacity() {
  return (lv_anim_exec_xcb_t)lv_obj_set_style_opa;
}

Animation::Path::Callback Animation::Path::Linear() {
  return lv_anim_path_linear;
}

Animation::Path::Callback Animation::Path::EaseIn() {
  return lv_anim_path_ease_in;
}

Animation::Path::Callback Animation::Path::EaseOut() {
  return lv_anim_path_ease_out;
}

Animation::Path::Callback Animation::Path::EaseInOut() {
  return lv_anim_path_ease_in_out;
}

Animation::Path::Callback Animation::Path::Overshoot() {
  return lv_anim_path_overshoot;
}

Animation::Path::Callback Animation::Path::Bounce() {
  return lv_anim_path_bounce;
}

Animation::Path::Callback Animation::Path::Step() { return lv_anim_path_step; }

Animation::Path::Callback Animation::Path::Bezier(int32_t x1, int32_t y1,
                                                  int32_t x2, int32_t y2) {
  // We need to capture these coordinates. Since LVGL path callbacks are just
  // function pointers, we can't capture in a raw C function.
  // The Animation class already has a proxy mechanism that uses user_data.
  // However, Animation::Path handles often return direct C function pointers.
  // To support custom Bezier curves, we'll use a lambda that captures the
  // coordinates.
  return [x1, y1, x2, y2](const lv_anim_t* a) -> int32_t {
    return lv_bezier3(lv_anim_path_linear(a), x1, y1, x2, y2);
  };
}

Animation& Animation::set_completed_cb(CompletedCallback cb) {
  if (!user_data_) user_data_ = std::make_unique<CallbackData>();
  user_data_->completed_cb = cb;
  return *this;
}

Animation& Animation::set_deleted_cb(std::function<void()> cb) {
  if (!user_data_) user_data_ = std::make_unique<CallbackData>();
  user_data_->deleted_cb = cb;
  return *this;
}

AnimationHandle Animation::start() {
  if (user_data_) {
    // Clone the callback data for this specific animation instance
    // detailed ownership management requires heap alloc that will be freed by
    // deleted_cb
    CallbackData* runtime_data = new CallbackData(*user_data_);
    lv_anim_set_user_data(ptr_, runtime_data);
    lv_anim_set_deleted_cb(ptr_, deleted_cb_proxy);

    if (user_data_->exec_cb) {
      lv_anim_set_custom_exec_cb(ptr_, exec_cb_proxy);
    }
    if (user_data_->path_cb) {
      lv_anim_set_path_cb(ptr_, path_cb_proxy);
    }
    if (user_data_->completed_cb) {
      lv_anim_set_completed_cb(ptr_, completed_cb_proxy);
    }
  }
  lv_anim_start(ptr_);
  return AnimationHandle(ptr_->var, ptr_->exec_cb);
}

void Animation::stop(void* var, lv_anim_exec_xcb_t exec_cb) {
  lv_anim_delete(var, exec_cb);
}

void Animation::stop(const Object& object) { stop(object.raw(), nullptr); }

}  // namespace lvgl
