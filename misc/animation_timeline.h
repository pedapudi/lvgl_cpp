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
   * @brief Set the time to wait before starting the animation.
   * Applies only when playing from the very start, or reverse from the very
   * end.
   * @param delay The delay time in milliseconds.
   */
  void set_delay(uint32_t delay);

  /**
   * @brief Make the animation timeline repeat itself.
   * @param count Repeat count or LV_ANIM_REPEAT_INFINITE. 0 to disable.
   */
  void set_repeat_count(uint32_t count);

  /**
   * @brief Set a delay before repeating the animation timeline.
   * @param delay Delay in milliseconds.
   */
  void set_repeat_delay(uint32_t delay);

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
   * @brief Get the start delay.
   * @return Delay in milliseconds.
   */
  uint32_t get_delay() const;

  /**
   * @brief Get the current progress.
   * @return Progress 0..65535.
   */
  uint16_t get_progress() const;

  /**
   * @brief Get the repeat count.
   * @return Repeat count.
   */
  uint32_t get_repeat_count() const;

  /**
   * @brief Get the repeat delay.
   * @return Delay in milliseconds.
   */
  uint32_t get_repeat_delay() const;

  /**
   * @brief Merge animations from another timeline into this one.
   * @param other The timeline to merge from.
   * @param extra_delay Extra delay to add for the merged animations.
   */
  void merge(const AnimationTimeline& other, int32_t extra_delay = 0);

  /**
   * @brief Set user data.
   * @param user_data The user data to set.
   */
  void set_user_data(void* user_data);

  /**
   * @brief Get user data.
   * @return The user data.
   */
  void* get_user_data() const;

  /**
   * @brief Detach the timeline from this object.
   * caller must manage memory of the timeline.
   * @return The raw lv_anim_timeline_t pointer.
   */
  lv_anim_timeline_t* detach();

  /**
   * @brief Access the underlying C struct.
   */
  lv_anim_timeline_t* raw() const { return timeline_; }

 private:
  lv_anim_timeline_t* timeline_ = nullptr;
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_ANIMATION_TIMELINE_H_
