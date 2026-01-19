# Design: LVGL Animation Timelines & Enhancements

**Tracking Issue:** #64

## 1. Context & Constraints

LVGL provides a robust animation system (`lv_anim_t`) and a timeline system (`lv_anim_timeline_t`) to orchestrate complex visual sequences. While `lvgl_cpp` currently wraps single animations, it lacks a wrapper for timelines.

A critical "Ground Truth" discovery in the LVGL codebase (`src/misc/lv_anim_timeline.c`) reveals a dangerous limitation: **`lv_anim_timeline_del()` does NOT execute the `deleted_cb` of the contained animations.**

### The Problem
The current `Animation` wrapper relies on `deleted_cb` to clean up heap-allocated C++ `std::function` closures (`CallbackData`). If we simply add these animations to a timeline using the standard API, the timeline will take a shallow copy of the implementation details, but upon destruction, it will **leak** the C++ callback data.

### Goals
1.  **Memory Safety**: Guarantee zero leaks for callbacks used within timelines.
2.  **Idiomatic C++**: Provide a RAII-compliant `AnimationTimeline` class.
3.  **Completeness**: Fill gaps in the `Animation` class API (e.g., `early_apply`, `start_cb`).

## 2. The Systemic Approach

To resolve the ownership impedance mismatch between C++ (RAII) and LVGL (manual C struct management), we must invert the ownership model for Timelines.

1.  **Container Ownership**: The `AnimationTimeline` wrapper must act as the *sole owner* of the callback resources for all its child animations.
2.  **Friendship Access**: To achieve this without exposing internal implementation details (`CallbackData`) to the end-user, `AnimationTimeline` will be a `friend` of the `Animation` class.
3.  **Snapshotting**: When an animation is added to a timeline, we capture a snapshot of its configuration and callback closures. These are stored in a `std::vector` inside the `AnimationTimeline` instance.

## 3. Implementation Strategy

### 3.1 Class Extensions: `Animation`
We need to enhance the existing `Animation` class to support the new features and friendship.

```cpp
namespace lvgl {

class AnimationTimeline; // Forward declaration

class Animation {
  friend class AnimationTimeline; // Grant access to user_data_

 public:
  // ... existing API ...

  /** @brief Apply start value immediately, even if there is a delay. */
  Animation& set_early_apply(bool en);
  
  /** @brief Set a callback to run when animation starts (after delay). */
  Animation& set_start_cb(std::function<void()> cb);
  
  /** @brief Calculate and set duration based on speed (pixels/sec). */
  Animation& set_duration_from_speed(uint32_t speed, int32_t start, int32_t end);
  
  /** @brief Get the internal C struct (const access). */
  const lv_anim_t* raw() const { return &anim_; }

 private:
  // ... existing private data ...
};
}
```

### 3.2 New Class: `AnimationTimeline`

This class wraps `lv_anim_timeline_t` and manages the lifecycle of callback data.

```cpp
#include "animation.h"

namespace lvgl {

class AnimationTimeline {
 public:
  AnimationTimeline();
  ~AnimationTimeline();

  // Non-copyable to prevent ambiguous ownership of callbacks
  AnimationTimeline(const AnimationTimeline&) = delete;
  AnimationTimeline& operator=(const AnimationTimeline&) = delete;
  
  // Move-constructible
  AnimationTimeline(AnimationTimeline&& other) noexcept;
  AnimationTimeline& operator=(AnimationTimeline&& other) noexcept;

  /**
   * @brief Add an animation to the timeline.
   * @param start_time Start time in milliseconds relative to timeline start.
   * @param anim The animation template. Its callbacks are cloned.
   */
  void add(uint32_t start_time, const Animation& anim);

  void start();
  void stop(); // Alias for lv_anim_timeline_stop
  void pause(); // Alias for lv_anim_timeline_pause
  void set_reverse(bool reverse);
  void set_progress(uint16_t progress);
  
  // Timeline global configuration
  void set_repeat_count(uint32_t cnt);
  void set_repeat_delay(uint32_t delay);

  // Getters
  uint32_t get_playtime() const;
  bool get_reverse() const;
  uint16_t get_progress() const;

 private:
  lv_anim_timeline_t* timeline_ = nullptr;

  // Owns the callback data for all animations in this timeline.
  // When the timeline is destroyed, this vector is cleared, freeing all closures.
  std::vector<std::unique_ptr<Animation::CallbackData>> resources_;
};

} // namespace lvgl
```

### 3.3 Internal Logic: `add()`

The `add` method is the critical piece. It must:
1.  Check if `anim` has `user_data_` (callbacks).
2.  If yes, `new` a copy of `CallbackData`.
3.  Store the `unique_ptr` in `resources_`.
4.  Configure the `lv_anim_t` to point to this new heap-allocated data.
5.  **Crucially**: Ensure `deleted_cb` is *not* set on the `lv_anim_t` passed to `lv_anim_timeline_add`, because the timeline won't call it anyway, and we rely on `resources_` destructor for cleanup.

```cpp
void AnimationTimeline::add(uint32_t start_time, const Animation& anim) {
  // 1. Copy the C struct
  lv_anim_t a = *anim.raw(); // Shallow copy

  // 2. Handle callbacks
  if (anim.user_data_) {
    // Clone the callback data
    auto data_copy = std::make_unique<Animation::CallbackData>(*anim.user_data_);
    
    // Update the C struct to point to our owned copy
    a.user_data = data_copy.get();
    
    // Transfer ownership to the timeline
    resources_.push_back(std::move(data_copy));
    
    // Ensure proxies are set (they should be already in 'a', but let's be safe)
    // We do NOT need to set deleted_cb, because the timeline won't call it.
    // Our vector handles destruction.
  }
  
  // 3. Add to LVGL timeline
  lv_anim_timeline_add(timeline_, start_time, &a);
}
```

## 4. Implementation Plan

### Phase 1: Enhancement of `Animation`
- [x] Modify `lvgl_cpp/misc/animation.h`:
    - [x] Add `friend class AnimationTimeline`.
    - [x] Add declarations for `set_early_apply`, `set_start_cb`, `set_duration_from_speed`.
- [x] Modify `lvgl_cpp/misc/animation.cpp`:
    - [x] Implement the new methods.
    - [x] Verify `lv_anim_set_start_cb` usage (available since v8.3+).

### Phase 2: Implementation of `AnimationTimeline`
- [x] Create `lvgl_cpp/misc/animation_timeline.h`:
    - [x] Define class structure.
- [x] Create `lvgl_cpp/misc/animation_timeline.cpp`:
    - [x] Implement `add` logic with resource cloning.
    - [x] Implement wrapper functions (`start`, `stop`, `set_reverse`, etc.).
    - [x] Implement Destructor (`lv_anim_timeline_del`).

### Phase 3: Validation
- [x] Create `lvgl_cpp/tests/test_animation_timeline.cpp`.
- [x] **Leak Test**:
    - [x] Define a custom "destructor tracking" struct.
    - [x] Capture it in a lambda passed to an `Animation` in a `Timeline`.
    - [x] Run the timeline, then destroy it.
    - [x] Assert the tracker count is 0.

## 5. References

1.  **LVGL Timeline API**: `src/misc/lv_anim_timeline.h`
2.  **Memory Analysis**: Validated against `lv_anim_timeline_del` implementation in LVGL 9.1.
3.  **C++ Best Practices**: Meyers, *Effective Modern C++* (Item 19: Use `std::shared_ptr` for shared-ownership resource management, though here `unique_ptr` in a container is sufficient).
