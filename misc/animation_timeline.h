#ifndef LVGL_CPP_MISC_ANIMATION_TIMELINE_H_
#define LVGL_CPP_MISC_ANIMATION_TIMELINE_H_

#include <cstdint>
#include <memory>
#include <vector>

#include "animation.h"
#include "lvgl.h"

namespace lvgl {

class AnimationTimeline {
 public:
  AnimationTimeline();
  ~AnimationTimeline();

  AnimationTimeline(const AnimationTimeline&) = delete;
  AnimationTimeline& operator=(const AnimationTimeline&) = delete;

  AnimationTimeline(AnimationTimeline&& other) noexcept;
  AnimationTimeline& operator=(AnimationTimeline&& other) noexcept;

  void add(uint32_t start_time, const Animation& anim);

  void start();
  void pause();

  // Helpers
  void stop();  // Implemented as pause + reset

  void set_reverse(bool reverse);
  void set_progress(uint16_t progress);

  void set_delay(uint32_t delay);
  void set_repeat_count(uint32_t cnt);
  void set_repeat_delay(uint32_t delay);

  uint32_t get_playtime() const;
  bool get_reverse() const;
  uint16_t get_progress() const;

  uint32_t get_delay() const;
  uint32_t get_repeat_count() const;
  uint32_t get_repeat_delay() const;

 private:
  lv_anim_timeline_t* timeline_ = nullptr;
  std::vector<std::unique_ptr<Animation::CallbackData>> resources_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_ANIMATION_TIMELINE_H_
