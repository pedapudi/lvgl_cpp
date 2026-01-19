#ifndef LVGL_CPP_MISC_ANIMATION_TIMELINE_H_
#define LVGL_CPP_MISC_ANIMATION_TIMELINE_H_

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

  /**
   * @brief Add an animation to the timeline.
   * @param anim The animation to add. It will be copied, and its callbacks
   * cloned.
   * @param start_time The start time on the timeline in milliseconds.
   */
  void add(Animation& anim, uint32_t start_time);

  /**
   * @brief Start the animation timeline.
   * @return Total time spent in animation timeline.
   */
  uint32_t start();

  /**
   * @brief Pause the animation timeline.
   */
  void pause();

  /**
   * @brief Set the playback direction.
   * @param reverse true for reverse playback.
   */
  void set_reverse(bool reverse);

  /**
   * @brief Set the progress of the timeline.
   * @param progress Progress 0..65535 map to 0..100%.
   */
  void set_progress(uint16_t progress);

  /**
   * @brief Get the total playtime.
   * @return Time in milliseconds.
   */
  uint32_t get_playtime() const;

  /**
   * @brief Get the playback direction.
   * @return true if reverse.
   */
  bool get_reverse() const;

  /**
   * @brief Get the current progress.
   * @return Progress 0..65535.
   */
  uint16_t get_progress() const;

  /**
   * @brief Access the underlying C struct.
   */
  lv_anim_timeline_t* raw() const { return timeline_; }

 private:
  lv_anim_timeline_t* timeline_ = nullptr;
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_ANIMATION_TIMELINE_H_
