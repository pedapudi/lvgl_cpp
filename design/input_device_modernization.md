# Technical Design: Input Device Modernization

This document outlines the strategy for improving `lv_indev` support in `lvgl_cpp`, focusing on gesture recognition and idiomatic event handling.

## 1. Context and Motivation

The current `InputDevice` wrapper covers basic setup (polling, type assignment) but lacks support for LVGL v9's advanced interaction features, particularly **Gestures** and **Multi-Touch**. Currently, users must drop down to C to handle complex gestures or two-finger interactions.

## 2. Engineering Principles

1.  **Facet-Based Access**: Input device features should be grouped into facets (e.g., `gestures()`, `config()`) to avoid monolithic class bloat.
2.  **Zero-Overhead Callbacks**: Use the established trampoline pattern to provide type-safe C++ callbacks.
3.  **RAII for Dynamic Buffers**: Ensure that any buffered input data or custom gesture recognizers are safely managed.

## 3. Proposed Architecture

### 3.1. The Gesture Facet
We will introduce a `GestureProxy` (accessible via `indev.gestures()`) that provides high-level access to gesture data.

```cpp
auto touch = PointerInput::get_act();
touch->gestures().on_swipe([](SwipeEvent& e) {
    if (e.direction() == Direction::Left) {
        // ...
    }
});
```

### 3.2. Gesture Event Abstraction
A new `GestureEvent` class will wrap the raw `lv_event_t` when the code is `LV_EVENT_GESTURE`.

```cpp
class GestureEvent {
public:
    GestureType type() const;
    Direction direction() const;
    Point velocity() const;
    // ...
};
```

---

## 4. Implementation Plan

### Phase 1: Event DSL Expansion
- [ ] Add `GestureType` enum mapping to `lv_indev_gesture_type_t`.
- [ ] Implement `GestureEvent` wrapper class.
- [ ] Update `EventProxy` to include a typed `on_gesture` helper.

### Phase 2: Input Device Facets
- [ ] Create `GestureProxy` to manage gesture-specific state.
- [ ] Expose `gestures()` on `InputDevice` base class.

### Phase 3: Configuration Modernization
- [ ] Restore `set_scroll_throw` as an idiomatic property.
- [ ] Add `set_gesture_limit` and `set_long_press_time` to the fluent configuration API.

## 5. Verification Plan

### Automated Tests
- Simulate gestural input in `test_indev.cpp` using `lv_indev_send_event`.
- Verify that the C++ `on_gesture` callback receives the correct direction and velocity.

### Manual Verification
- Test on hardware (XIAO ESP32C6 + Round Display) to ensure swipes feel natural and responsive.
