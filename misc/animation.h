#ifndef LVGL_CPP_MISC_ANIMATION_H_
#define LVGL_CPP_MISC_ANIMATION_H_

#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>

#include "../core/object.h"  // IWYU pragma: export
#include "anim_exec_callback.h"
#include "anim_path_callback.h"
#include "lvgl.h"  // IWYU pragma: export

namespace lvgl {

class AnimationTimeline;
class Animation;

/**
 * @brief A handle to a running animation.
 *
 * This handle is returned by Animation::start() and allows checking if
 * the animation is still running or stopping it mid-flight.
 */
class AnimationHandle {
 public:
  AnimationHandle() = default;
  AnimationHandle(void* var, lv_anim_exec_xcb_t exec_cb);

  /**
   * @brief Check if the animation is currently running.
   */
  bool is_running() const;

  /**
   * @brief Stop the animation (delete it from the LVGL task list).
   */
  void stop();

 private:
  void* var_ = nullptr;
  lv_anim_exec_xcb_t exec_cb_ = nullptr;
};

/**
 * @brief An RAII wrapper for AnimationHandle that automatically stops
 * the animation when it goes out of scope.
 */
class ScopedAnimation {
 public:
  explicit ScopedAnimation(AnimationHandle handle) : handle_(handle) {}
  ~ScopedAnimation() {
    if (handle_.is_running()) {
      handle_.stop();
    }
  }

  ScopedAnimation(const ScopedAnimation&) = delete;
  ScopedAnimation& operator=(const ScopedAnimation&) = delete;

  ScopedAnimation(ScopedAnimation&& other) noexcept : handle_(other.handle_) {
    // Invalidate other handle mechanism? AnimationHandle copies are cheap/safe.
    // But ScopedAnimation owns the responsibility to stop.
    // If we move, the other shouldn't stop.
    // But AnimationHandle doesn't have a "null" state easily except manually
    // setting vars.
    // We need to implement a move ctor that creates a "null" handle in other.
    // But AnimationHandle fields are private.
    // Let's rely on detach() or make AnimationHandle more flexible?
    // Actually, AnimationHandle is copyable.
    // We need a way to clear `other`'s handle inside specific scope logic.
    // Since AnimationHandle fields are private, we can't clear them?
    // We can add a "reset()" to AnimationHandle or just allow ScopedAnimation
    // to default construct a handle (which is null).
    other.handle_ = AnimationHandle();
  }

  ScopedAnimation& operator=(ScopedAnimation&& other) noexcept {
    if (this != &other) {
      if (handle_.is_running()) handle_.stop();
      handle_ = other.handle_;
      other.handle_ = AnimationHandle();
    }
    return *this;
  }

  /**
   * @brief Detach the animation, allowing it to continue running.
   * @return The handle to the running animation.
   */
  AnimationHandle detach() {
    AnimationHandle h = handle_;
    handle_ = AnimationHandle();
    return h;
  }

  bool is_running() const { return handle_.is_running(); }
  void stop() { handle_.stop(); }

 private:
  AnimationHandle handle_;
};

class Animation {
  friend class AnimationTimeline;

 public:
  using ExecCallback = std::function<void(void*, int32_t)>;
  /**
   * @brief Type-safe execution callback for Objects.
   * Receives a temporary unmanaged Object wrapper.
   */
  using ObjectExecCallback = std::function<void(Object&, int32_t)>;

  using PathCallback = std::function<int32_t(const lv_anim_t*)>;
  using CompletedCallback = std::function<void()>;

  Animation();

  /** @brief Infinite repetition constant */
  static constexpr uint32_t RepeatInfinite = LV_ANIM_REPEAT_INFINITE;

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

  /**
   * @brief Construct an animation wrapping an existing C animation.
   * Note: The wrapper does NOT take ownership (will not delete/free).
   * @param anim Pointer to the existing lv_anim_t.
   */
  explicit Animation(lv_anim_t* anim);

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

  Animation& set_exec_cb(ObjectExecCallback cb);

  /**
   * @brief Set a type-safe execution callback for a specific Widget type.
   * Auto-casts the object to the requested type.
   *
   * @example
   * anim.set_exec_cb<lvgl::Image>([](lvgl::Image& img, int32_t val) {
   *     img.set_rotation(val);
   * });
   *
   * @tparam T The widget type (e.g., lvgl::Image, lvgl::Label).
   * @param cb The typed callback.
   */
  template <typename T>
  Animation& set_exec_cb(std::function<void(T&, int32_t)> cb) {
    return set_exec_cb([cb](Object& obj, int32_t v) {
      if (T* typed = dynamic_cast<T*>(&obj)) {
        cb(*typed, v);
      }
    });
  }

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

    /** @brief Bezier value shift constant (1024 = 2^10) */
    static constexpr uint32_t BezierValueShift = LV_BEZIER_VAL_SHIFT;

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

    /**
     * @brief Create a cubic Bezier path.
     * @param x1 Control point 1 X (0..1024)
     * @param y1 Control point 1 Y (0..1024)
     * @param x2 Control point 2 X (0..1024)
     * @param y2 Control point 2 Y (0..1024)
     * @return A PathCallback defined by the Bezier curve.
     */
    static Callback Bezier(int32_t x1, int32_t y1, int32_t x2, int32_t y2);

    /**
     * @brief Create a cubic Bezier path (SVG/CSS Standard).
     * This implementation matches standard SVG keySpline behavior,
     * interpolating the value range based on the curve.
     *
     * @param x1 Control point 1 X (0..1024)
     * @param y1 Control point 1 Y (0..1024)
     * @param x2 Control point 2 X (0..1024)
     * @param y2 Control point 2 Y (0..1024)
     * @return A PathCallback defined by the Bezier curve.
     */
    static Callback CubicBezier(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
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
   * @brief Set the duration of the animation using chrono.
   * @param duration Duration.
   */
  Animation& set_duration(std::chrono::milliseconds duration);

  /**
   * @brief Set a delay before starting the animation.
   * @param delay Delay in milliseconds.
   */
  Animation& set_delay(uint32_t delay);

  /**
   * @brief Set a delay before starting the animation using chrono.
   * @param delay Delay.
   */
  Animation& set_delay(std::chrono::milliseconds delay);

  /**
   * @brief Set the start and end values.
   * @param start Start value.
   * @param end End value.
   */
  Animation& set_values(int32_t start, int32_t end);

  /**
   * @brief Set the path (easing) function using a raw C callback.
   * @param path_cb The LVGL path callback (e.g., `lv_anim_path_linear`).
   */
  Animation& set_path_cb(lv_anim_path_cb_t path_cb);

  /**
   * @brief Set a C++ path (easing) callback (lambda/std::function).
   * Used for custom easing or capturing lambdas.
   * @param cb The `std::function` path callback.
   */
  Animation& set_path_cb(const PathCallback& cb);

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

  AnimationHandle start();

  /**
   * @brief Stop any animation running for a variable and execution callback.
   * @param var The variable/object.
   * @param exec_cb The execution callback.
   */
  static void stop(void* var, lv_anim_exec_xcb_t exec_cb);

  /**
   * @brief Stop all animations for an object.
   * @param object The object wrapper.
   */
  static void stop(const Object& object);

 private:
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

  lv_anim_t anim_;
  lv_anim_t* ptr_ = &anim_;  // Pointer to the active animation struct (either
                             // anim_ or external)
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_ANIMATION_H_
