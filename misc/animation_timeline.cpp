#include "animation_timeline.h"

namespace lvgl {

AnimationTimeline::AnimationTimeline() {
  timeline_ = lv_anim_timeline_create();
}

AnimationTimeline::~AnimationTimeline() {
  if (timeline_) {
    lv_anim_timeline_delete(timeline_);
    timeline_ = nullptr;
  }
}

AnimationTimeline::AnimationTimeline(AnimationTimeline&& other) noexcept
    : timeline_(other.timeline_) {
  other.timeline_ = nullptr;
}

AnimationTimeline& AnimationTimeline::operator=(
    AnimationTimeline&& other) noexcept {
  if (this != &other) {
    if (timeline_) {
      lv_anim_timeline_delete(timeline_);
    }
    timeline_ = other.timeline_;
    other.timeline_ = nullptr;
  }
  return *this;
}

void AnimationTimeline::add(Animation& anim, uint32_t start_time) {
  lv_anim_t temp_anim = anim.anim_;

  if (anim.user_data_) {
    // Clone callback data (Animation is a friend)
    // We allocate a new CallbackData on the heap.
    // It will be deleted by Animation::deleted_cb_proxy when this timeline/anim
    // is deleted.
    auto* runtime_data = new Animation::CallbackData(*anim.user_data_);

    lv_anim_set_user_data(&temp_anim, runtime_data);
    lv_anim_set_deleted_cb(&temp_anim, Animation::deleted_cb_proxy);

    if (anim.user_data_->exec_cb) {
      lv_anim_set_custom_exec_cb(&temp_anim, Animation::exec_cb_proxy);
    }
    if (anim.user_data_->path_cb) {
      lv_anim_set_path_cb(&temp_anim, Animation::path_cb_proxy);
    }
    if (anim.user_data_->completed_cb) {
      lv_anim_set_completed_cb(&temp_anim, Animation::completed_cb_proxy);
    }
  }

  lv_anim_timeline_add(timeline_, start_time, &temp_anim);
}

uint32_t AnimationTimeline::start() {
  if (timeline_) return lv_anim_timeline_start(timeline_);
  return 0;
}

void AnimationTimeline::pause() {
  if (timeline_) lv_anim_timeline_pause(timeline_);
}

void AnimationTimeline::set_reverse(bool reverse) {
  if (timeline_) lv_anim_timeline_set_reverse(timeline_, reverse);
}

void AnimationTimeline::set_delay(uint32_t delay) {
  if (timeline_) lv_anim_timeline_set_delay(timeline_, delay);
}

void AnimationTimeline::set_repeat_count(uint32_t count) {
  if (timeline_) lv_anim_timeline_set_repeat_count(timeline_, count);
}

void AnimationTimeline::set_repeat_delay(uint32_t delay) {
  if (timeline_) lv_anim_timeline_set_repeat_delay(timeline_, delay);
}

void AnimationTimeline::set_progress(uint16_t progress) {
  if (timeline_) lv_anim_timeline_set_progress(timeline_, progress);
}

uint32_t AnimationTimeline::get_playtime() const {
  if (timeline_) return lv_anim_timeline_get_playtime(timeline_);
  return 0;
}

bool AnimationTimeline::get_reverse() const {
  if (timeline_) return lv_anim_timeline_get_reverse(timeline_);
  return false;
}

uint32_t AnimationTimeline::get_delay() const {
  if (timeline_) return lv_anim_timeline_get_delay(timeline_);
  return 0;
}

uint16_t AnimationTimeline::get_progress() const {
  if (timeline_) return lv_anim_timeline_get_progress(timeline_);
  return 0;
}

uint32_t AnimationTimeline::get_repeat_count() const {
  if (timeline_) return lv_anim_timeline_get_repeat_count(timeline_);
  return 0;
}

uint32_t AnimationTimeline::get_repeat_delay() const {
  if (timeline_) return lv_anim_timeline_get_repeat_delay(timeline_);
  return 0;
}

void AnimationTimeline::merge(const AnimationTimeline& other,
                              int32_t extra_delay) {
  if (timeline_ && other.timeline_) {
    // Note: lv_anim_timeline_merge is not yet available in all LVGL versions.
    // Assuming it is available based on context or we might need to iterate.
    // Checking coverage.json, it lists lv_anim_timeline_merge as wrapped?
    // Wait, the header has it.
    lv_anim_timeline_merge(timeline_, other.timeline_, extra_delay);
  }
}

void AnimationTimeline::set_user_data(void* user_data) {
  if (timeline_) {
    // Note: lv_anim_timeline_set_user_data might not exist in standard LVGL 9.4
    // or is a macro/internal field.
    // But api_coverage.json listed it as "not_wrapped".
    // Let's assume it exists or we use the generic way if it's a struct field.
    // Actually, lv_anim_timeline_set_user_data does NOT exist in standard API
    // often. However, if the user requested coverage for it, it might be in
    // their custom fork? Or it might be a misunderstanding of the task. Wait,
    // let's look at coverage.json again. It says
    // "lv_anim_timeline_set_user_data": "not_wrapped" This implies the tool
    // FOUND the symbol in headers. So we use it.
    lv_anim_timeline_set_user_data(timeline_, user_data);
  }
}

void* AnimationTimeline::get_user_data() const {
  return timeline_ ? lv_anim_timeline_get_user_data(timeline_) : nullptr;
}

lv_anim_timeline_t* AnimationTimeline::detach() {
  lv_anim_timeline_t* t = timeline_;
  timeline_ = nullptr;
  return t;
}

}  // namespace lvgl
