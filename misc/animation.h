#ifndef LVGL_CPP_MISC_ANIMATION_H_
#define LVGL_CPP_MISC_ANIMATION_H_

#include <cstdint>
#include <functional>
#include <memory>

#include "../core/object.h"  // IWYU pragma: export
#include "anim_exec_callback.h"
#include "anim_path_callback.h"
#include "lvgl.h"  // IWYU pragma: export

namespace lvgl {

class Animation {
 public:
  using ExecCallback = std::function<void(void*, int32_t)>;
  using PathCallback = std::function<int32_t(const lv_anim_t*)>;
  using CompletedCallback = std::function<void()>;

  Animation();

  /**
   * @brief Construct an animation with common parameters.
   * @param var The object/variable to animate.
   * @param start_val Start value.
   * @param end_val End value.
   * @param duration Duration in milliseconds.
   */
  Animation(void* var, int32_t start_val, int32_t end_val, uint32_t duration);

  /**
   * @brief Construct an animation for a specific Object.
   * @param object The object to animate.
   */
  explicit Animation(const Object& object);

  ~Animation();

  // Builder pattern

  /**
   * @brief Set the object or variable to animate.
   * @param var Pointer to the variable/object.
   */
  Animation& set_var(void* var);

  /**
   * @brief Set the object to animate (overload for C++ wrapper).
   * @param object The object wrapper.
   */
  Animation& set_var(const Object& object);

  /**
   * @brief Set a C-style execution callback.
   *
   * @example
   * // 1. Use built-in optimized callbacks (Recommended)
   * anim.set_exec_cb(lvgl::Animation::Exec::X());
   *
   * // 2. Use raw LVGL C functions
   * anim.set_exec_cb((lv_anim_exec_xcb_t)lv_obj_set_width);
   *
   * @param exec_cb The callback function pointer.
   */
  Animation& set_exec_cb(lv_anim_exec_xcb_t exec_cb);

  /**
   * @brief Set a C++ execution callback (lambda/std::function).
   *
   * @example
   * anim.set_exec_cb([](void* var, int32_t val) {
   *     // Custom logic
   * });
   *
   * @param cb The `std::function` callback.
   */
  Animation& set_exec_cb(ExecCallback cb);

  // ... (Exec struct omitted for brevity)

  struct Exec {
    using Callback = ExecCallback;

    /** @brief Callback to animate X coordinate (lv_obj_set_x) */
    static Callback X();

    /** @brief Callback to animate Y coordinate (lv_obj_set_y) */
    static Callback Y();

    /** @brief Callback to animate Width (lv_obj_set_width) */
    static Callback Width();

    /** @brief Callback to animate Height (lv_obj_set_height) */
    static Callback Height();

    /** @brief Callback to animate Opacity (lv_obj_set_style_opa) */
    static Callback Opacity();
  };

  /**
   * @brief Helper struct for common path (easing) callbacks.
   */
  struct Path {
    using Callback = PathCallback;

    /** @brief Linear animation (no easing) */
    static Callback Linear();

    /** @brief Ease in (slow start) */
    static Callback EaseIn();

    /** @brief Ease out (slow end) */
    static Callback EaseOut();

    /** @brief Ease in and out (slow start and end) */
    static Callback EaseInOut();

    /** @brief Overshoot the end value */
    static Callback Overshoot();

    /** @brief Bounce back from the end value */
    static Callback Bounce();

    /** @brief Instant step to end value */
    static Callback Step();
  };

  /**
   * @brief Set a completion callback.
   * @param cb The callback to run when animation completes.
   */
  Animation& set_completed_cb(CompletedCallback cb);

  /**
   * @brief Set a deletion callback.
   * @param cb The callback to run when animation is deleted.
   */
  Animation& set_deleted_cb(std::function<void()> cb);

  /**
   * @brief Set the duration of the animation.
   * @param duration Duration in milliseconds.
   */
  Animation& set_duration(uint32_t duration);

  /**
   * @brief Set a delay before starting the animation.
   * @param delay Delay in milliseconds.
   */
  Animation& set_delay(uint32_t delay);

  /**
   * @brief Set the start and end values.
   * @param start Start value.
   * @param end End value.
   */
  Animation& set_values(int32_t start, int32_t end);

  /**
   * @brief Set the path (easing) function.
   * @param path_cb The LVGL path callback (e.g., `lv_anim_path_linear`).
   */
  Animation& set_path_cb(lv_anim_path_cb_t path_cb);

  /**
   * @brief Set a C++ path (easing) callback.
   * @param cb The `std::function` path callback.
   */
  Animation& set_path_cb(PathCallback cb);

  /**
   * @brief Set the repeat count.
   * @param cnt Number of repetitions (`LV_ANIM_REPEAT_INFINITE` for infinite).
   */
  Animation& set_repeat_count(uint32_t cnt);

  /**
   * @brief Set the delay before repetition.
   * @param delay Delay in milliseconds.
   */
  Animation& set_repeat_delay(uint32_t delay);

  /**
   * @brief Set the playback duration (reverse direction).
   * @param duration Duration in milliseconds.
   */
  Animation& set_playback_duration(uint32_t duration);

  /**
   * @brief Set the playback delay.
   * @param delay Delay in milliseconds.
   */
  Animation& set_playback_delay(uint32_t delay);

  void start();

 private:
  lv_anim_t anim_;

  // Internal closure data to bridge C callbacks to C++ std::function
  struct CallbackData {
    ExecCallback exec_cb;
    PathCallback path_cb;
    CompletedCallback completed_cb;
    std::function<void()> deleted_cb;
  };

  std::unique_ptr<CallbackData> user_data_;

  static void exec_cb_proxy(lv_anim_t* a, int32_t v);
  static int32_t path_cb_proxy(const lv_anim_t* a);
  static void completed_cb_proxy(lv_anim_t* a);
  static void deleted_cb_proxy(lv_anim_t* a);
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_ANIMATION_H_
