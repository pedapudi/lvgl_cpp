#include "animation.h"

namespace lvgl {

Animation::Animation() { lv_anim_init(&anim_); }

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

void Animation::exec_cb_proxy(lv_anim_t* a, int32_t v) {
  CallbackData* data = static_cast<CallbackData*>(a->user_data);
  if (data && data->exec_cb) {
    data->exec_cb(a->var, v);
  }
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
  user_data_->exec_cb = cb;
  // We don't set user_data on anim_ yet, we do it at start() to allow multiple
  // instances
  return *this;
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

void Animation::start() {
  if (user_data_) {
    // Clone the callback data for this specific animation instance
    // detailed ownership management requires heap alloc that will be freed by
    // deleted_cb
    CallbackData* runtime_data = new CallbackData(*user_data_);
    lv_anim_set_user_data(&anim_, runtime_data);
    lv_anim_set_deleted_cb(&anim_, deleted_cb_proxy);

    if (user_data_->exec_cb) {
      lv_anim_set_custom_exec_cb(&anim_, exec_cb_proxy);
    }
    if (user_data_->completed_cb) {
      lv_anim_set_completed_cb(&anim_, completed_cb_proxy);
    }
  }
  lv_anim_start(&anim_);
}

}  // namespace lvgl
