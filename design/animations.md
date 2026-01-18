# Research Report: LVGL Animations & Timelines in C++

## Executive Summary

LVGL's animation system (`lv_anim_t`) and timeline system (`lv_anim_timeline_t`) provide powerful tools for creating dynamic user interfaces. The `lvgl_cpp` library currently wraps single animations via the `Animation` class but lacks a wrapper for Timelines.

This report analyzes the underlying mechanisms of LVGL animations, identifies critical resource management challenges when using Timelines with C++ callbacks, and proposes a design for an idiomatic `AnimationTimeline` C++ wrapper that safely manages lifecycle and memory.

## 1. LVGL Animation Mechanics

### 1.1 Single Animations (`lv_anim_t`)
Core animations are struct-based. Users initialize an `lv_anim_t` on the stack, configure it, and start it. The global animation timer handles execution.
- **Callbacks**: Relies on function pointers.
- **User Data**: `user_data` pointer allows attaching context.
- **Lifecycle**: `deleted_cb` allows cleanup of `user_data`. This is the hook `lvgl_cpp::Animation` uses to destruct C++ closures.

### 1.2 Animation Timelines (`lv_anim_timeline_t`)
Timelines orchestrate multiple animations.
- **Adding Animations**: `lv_anim_timeline_add` copies the animation descriptor.
- **Critical Limitation**: When `lv_anim_timeline_del()` is called, it frees the timeline memory but **does NOT call `deleted_cb`** on the contained animation descriptors.
    - **Implication**: Any C++ `CallbackData` attached to animations in a timeline will **leak** if relying solely on the standard `deleted_cb` mechanism.

## 2. Existing `lvgl_cpp::Animation` Wrapper

The existing `Animation` class uses a builder pattern and handles C++ `std::function` callbacks by allocating a `CallbackData` struct on the heap. It relies on `deleted_cb` for cleanup, which works for single animations but fails for Timelines.

## 3. Proposed Design: `AnimationTimeline` Wrapper

To support Timelines with safe C++ callbacks, the `AnimationTimeline` wrapper must assume ownership of all callback resources.

### 3.1 Class Structure

```cpp
class AnimationTimeline {
 public:
  AnimationTimeline();
  ~AnimationTimeline();

  // Add an animation to the timeline
  void add(uint32_t start_time, const Animation& anim_template);

  // Control
  uint32_t start();
  void pause();
  
  void set_reverse(bool reverse);
  void set_progress(uint16_t progress);
  
  // Timeline global configuration
  void set_delay(uint32_t delay);
  void set_repeat_count(uint32_t cnt);
  void set_repeat_delay(uint32_t delay);
  
  // Properties
  uint32_t get_playtime() const;
  bool get_reverse() const;
  uint32_t get_delay() const;
  uint16_t get_progress() const;
  uint32_t get_repeat_count() const;
  uint32_t get_repeat_delay() const;

 private:
  lv_anim_timeline_t* timeline_;
  
  // Store callback data for all animations in this timeline to prevent leaks
  // and ensure they stay alive as long as the timeline exists.
  std::vector<std::unique_ptr<Animation::CallbackData>> callback_resources_;
};
```

### 3.2 Implementation Constraints & Safety Noted
1.  **Append-Only**: The `lv_anim_timeline` API allows adding animations but supports **neither removal nor modification** of existing items. The C++ wrapper reflects this immutable-history nature.
2.  **No Merge Support**: `lv_anim_timeline_merge` performs a bitwise copy of animation descriptors. This is unsafe for C++ closures (causing double-free or use-after-free on `user_data`). Thus, `merge` will not be exposed.
3.  **Ownership**: The `AnimationTimeline` instance exclusively owns the `CallbackData` for its children.

## 4. Usage Example

```cpp
void CreateIntroAnimation(lv_obj_t* label, lv_obj_t* button) {
  auto timeline = std::make_shared<AnimationTimeline>();

  Animation label_anim;
  label_anim.set_var(label).set_values(-100, 0).set_exec_cb(Animation::Exec::Y());

  timeline->add(0, label_anim);
  timeline->start();
}
```

## 5. Recommendations for Existing `Animation` Class

While researching `lv_anim.h`, several useful features were identified that are currently missing from the C++ `Animation` wrapper. These should be added to provide full API coverage.

### 5.1 Missing Features
-   **Early Apply**: `lv_anim_set_early_apply(lv_anim_t*, bool)`. Critical for animations with delays where the start value should be visible immediately.
-   **Start Callback**: `lv_anim_set_start_cb`. useful for triggering logic exactly when the animation begins (post-delay).
-   **Relative Values**: `lv_anim_set_get_value_cb`. Allows animations to properly handle "relative" moves (e.g. `current + 10`).
-   **Speed-based Duration**: `lv_anim_speed_to_time` helpers. Allow defining animation by speed (pixels/sec) rather than fixed duration.

### 5.2 Proposed Additions
```cpp
class Animation {
  // ... existing ...
  
  /** @brief Apply start value immediately, even if there is a delay. */
  Animation& set_early_apply(bool en);
  
  /** @brief Set a callback to run when animation starts (after delay). */
  Animation& set_start_cb(std::function<void()> cb);
  
  /** @brief Calculate and set duration based on speed and current values. */
  Animation& set_duration_from_speed(uint32_t speed, int32_t start, int32_t end);
  
  // ... 
};
```

## 6. References

1.  **LVGL Documentation - Animation**: [https://docs.lvgl.io/9.4/details/main-modules/animation.html](https://docs.lvgl.io/9.4/details/main-modules/animation.html)
2.  **API Documentation**: 
    - [lv_anim.h](https://docs.lvgl.io/9.4/API/misc/lv_anim_h.html)
    - [lv_anim_timeline.h](https://docs.lvgl.io/9.4/API/misc/lv_anim_timeline_h.html)
3.  **LVGL Source Code**: `src/misc/lv_anim_timeline.c` (confirmed `deleted_cb` is not called on deletion).
4.  **Existing Wrapper**: `lvgl_cpp/misc/animation.h`
