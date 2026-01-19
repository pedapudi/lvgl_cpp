#include "animation_timeline.h"

namespace lvgl {

AnimationTimeline::AnimationTimeline() {
  timeline_ = lv_anim_timeline_create();
}

AnimationTimeline::~AnimationTimeline() {
  if (timeline_) {
    lv_anim_timeline_delete(timeline_);
  }
}

AnimationTimeline::AnimationTimeline(AnimationTimeline&& other) noexcept
    : timeline_(other.timeline_), resources_(std::move(other.resources_)) {
  other.timeline_ = nullptr;
}

AnimationTimeline& AnimationTimeline::operator=(
    AnimationTimeline&& other) noexcept {
  if (this != &other) {
    if (timeline_) lv_anim_timeline_delete(timeline_);
    timeline_ = other.timeline_;
    resources_ = std::move(other.resources_);
    other.timeline_ = nullptr;
  }
  return *this;
}

void AnimationTimeline::add(uint32_t start_time, const Animation& anim) {
  lv_anim_t a = *anim.raw();  // Shallow copy

  if (anim.user_data_) {
    auto data_copy =
        std::make_unique<Animation::CallbackData>(*anim.user_data_);
    a.user_data = data_copy.get();

    // CRITICAL: Disable deleted_cb for the copy in the timeline
    // The timeline won't call it anyway, but we want to be explicit
    // that we manage ownership via 'resources_'.
    lv_anim_set_deleted_cb(&a, nullptr);

    resources_.push_back(std::move(data_copy));
  }

  lv_anim_timeline_add(timeline_, start_time, &a);
}

void AnimationTimeline::start() { lv_anim_timeline_start(timeline_); }

void AnimationTimeline::pause() { lv_anim_timeline_pause(timeline_); }

void AnimationTimeline::stop() {
  lv_anim_timeline_pause(timeline_);
  lv_anim_timeline_set_progress(timeline_, 0);
}

void AnimationTimeline::set_reverse(bool reverse) {
  lv_anim_timeline_set_reverse(timeline_, reverse);
}

void AnimationTimeline::set_progress(uint16_t progress) {
  lv_anim_timeline_set_progress(timeline_, progress);
}

uint32_t AnimationTimeline::get_playtime() const {
  return lv_anim_timeline_get_playtime(timeline_);
}

bool AnimationTimeline::get_reverse() const {
  return lv_anim_timeline_get_reverse(timeline_);
}

uint16_t AnimationTimeline::get_progress() const {
  return lv_anim_timeline_get_progress(timeline_);
}

void AnimationTimeline::set_delay(uint32_t delay) {
  lv_anim_timeline_set_delay(timeline_, delay);
}

void AnimationTimeline::set_repeat_count(uint32_t cnt) {
  lv_anim_timeline_set_repeat_count(timeline_, cnt);
}

void AnimationTimeline::set_repeat_delay(uint32_t delay) {
  lv_anim_timeline_set_repeat_delay(timeline_, delay);
}

uint32_t AnimationTimeline::get_delay() const {
  return lv_anim_timeline_get_delay(timeline_);
}

uint32_t AnimationTimeline::get_repeat_count() const {
  return lv_anim_timeline_get_repeat_count(timeline_);
}

uint32_t AnimationTimeline::get_repeat_delay() const {
  return lv_anim_timeline_get_repeat_delay(timeline_);
}

}  // namespace lvgl
