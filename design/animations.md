# Research Report: LVGL Animations & Timelines in C++

## Executive Summary

LVGL's animation system (`lv_anim_t`) and timeline system (`lv_anim_timeline_t`) provide powerful tools for creating dynamic user interfaces. The `lvgl_cpp` library currently wraps single animations via the `Animation` class but lacks a wrapper for Timelines.

This report analyzes the underlying mechanisms of LVGL animations, identifies critical resource management challenges when using Timelines with C++ callbacks, and proposes a design for an idiomatic `AnimationTimeline` C++ wrapper that safely manages lifecycle and memory.

## 1. LVGL Animation Mechanics

### 1.1 Single Animations (`lv_anim_t`)
Core animations are struct-based. Users initialize an `lv_anim_t` on the stack, configure it, and start it. The global animation timer handles execution.
- **Callbacks**: Relies on function pointers (`exec_cb`, `path_cb`, `start_cb`, `completed_cb`, `deleted_cb`).
- **User Data**: `user_data` pointer allows attaching context.
- **Lifecycle**: When an animation finishes or is deleted, `deleted_cb` is called, allowing cleanup of `user_data`. This is the hook `lvgl_cpp::Animation` uses to destruct C++ closures.

### 1.2 Animation Timelines (`lv_anim_timeline_t`)
Timelines orchestrate multiple animations.
- **Adding Animations**: `lv_anim_timeline_add(timeline, start_time, &anim_template)` makes a **copy** of the animation descriptor.
- **Execution**: The timeline has a single master animator that moves the "head" of the timeline. It manually executes callbacks of the child animations based on the current time.
- **Critical Limitation**: When `lv_anim_timeline_del()` is called, it frees the timeline memory but **does NOT call `deleted_cb`** on the contained animation descriptors.
    - **Implication**: Any C++ `CallbackData` attached to animations in a timeline will **leak** if relying solely on the standard `deleted_cb` mechanism.

## 2. Existing `lvgl_cpp::Animation` Wrapper

The existing `Animation` class in `misc/animation.h` uses a builder pattern and handles C++ `std::function` callbacks by:
1.  Allocating a `CallbackData` struct on the heap when `start()` is called.
2.  Attaching this struct to `lv_anim_t::user_data`.
3.  Setting `lv_anim_t::deleted_cb` to a proxy that `delete`s the `CallbackData`.

This works perfectly for single animations but is incompatible with Timelines because Timelines usually don't trigger the `deleted_cb`.

## 3. Proposed Design: `AnimationTimeline` Wrapper

To support Timelines with safe C++ callbacks, the `AnimationTimeline` wrapper must assume ownership of all callback resources.

### 3.1 Design Goals
-   **RAII**: The generic C++ wrapper manages the lifetime of `lv_anim_timeline_t`.
-   **Resource Ownership**: It must store and own the `CallbackData` for every animation added to it.
-   **Ease of Use**: Allow adding `Animation` objects (templates) directly.

### 3.2 Class Structure

```cpp
class AnimationTimeline {
 public:
  AnimationTimeline();
  ~AnimationTimeline();

  // Add an animation to the timeline
  void add(uint32_t start_time, const Animation& anim_template);

  // Control
  void start();
  void pause();
  void set_reverse(bool reverse);
  void set_progress(uint16_t progress);
  
  // Properties
  uint32_t get_playtime() const;
  bool get_reverse() const;

 private:
  lv_anim_timeline_t* timeline_;
  
  // Store callback data for all animations in this timeline to prevent leaks
  // and ensure they stay alive as long as the timeline exists.
  std::vector<std::unique_ptr<Animation::CallbackData>> callback_resources_;
};
```

### 3.3 Implementation Strategy

When `add` is called:
1.  Access the `CallbackData` from the `Animation` template. (Requires `Animation` to expose internal state to `AnimationTimeline`, typically via `friend` declaration).
2.  **Clone** the `CallbackData` onto the heap.
3.  Store the unique pointer in `callback_resources_`.
4.  Create a temporary `lv_anim_t` struct based on `anim_template`.
5.  Set the temporary struct's `user_data` to the cloned `CallbackData` pointer.
6.  Set `custom_exec_cb` etc. to the static proxy functions in `Animation` (e.g., `Animation::exec_cb_proxy`).
7.  Call `lv_anim_timeline_add(timeline_, start_time, &temp_anim)`.

When `AnimationTimeline` is destroyed:
1.  `lv_anim_timeline_delete(timeline_)` is called.
2.  `callback_resources_` vector is destroyed, automatically freeing all `CallbackData` instances.

### 3.4 Required Specifics

**Friend Declaration in `Animation`**:
```cpp
// In misc/animation.h
class Animation {
  friend class AnimationTimeline; // Allow access to user_data_ and proxies
  // ...
};
```

**Callback Handling**:
Since `lv_anim_timeline` copies the `lv_anim_t`, the `user_data` pointer is copied. The `AnimationTimeline` class ensures the pointee remains valid.

## 4. Usage Example

```cpp
void CreateIntroAnimation(lv_obj_t* label, lv_obj_t* button) {
  // Create a timeline wrapper (should likely be stored in a class member or managed pointer)
  auto timeline = std::make_shared<AnimationTimeline>();

  // Define animations using the builder
  Animation label_anim;
  label_anim.set_var(label)
            .set_values(-100, 0)
            .set_duration(500)
            .set_exec_cb(Animation::Exec::Y())
            .set_path_cb(Animation::Path::Overshoot());

  Animation btn_fade;
  btn_fade.set_var(button)
          .set_values(0, 255)
          .set_duration(300)
          .set_exec_cb(Animation::Exec::Opacity());

  // Add to timeline
  timeline->add(0, label_anim);
  timeline->add(200, btn_fade); // Start 200ms later

  // Start
  timeline->start();
  
  // Note: 'timeline' must be kept alive!
}
```

## 5. References

1.  **LVGL Documentation - Animation**: [https://docs.lvgl.io/9.4/details/main-modules/animation.html](https://docs.lvgl.io/9.4/details/main-modules/animation.html)
2.  **LVGL Source Code**: `src/misc/lv_anim_timeline.c` (confirmed `deleted_cb` is not called on deletion).
3.  **Existing Wrapper**: `lvgl_cpp/misc/animation.h`
