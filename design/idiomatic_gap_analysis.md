# Idiomatic Gap Analysis: Style Selectors & Typed Callbacks

## 1. Executive Summary

This document addresses two critical ergonomic gaps in `lvgl_cpp` identified in Issues #168 and #170. These gaps represent friction points where the C++ abstraction leaks, forcing users to interact with raw C types (`void*`, `lv_style_selector_t`) or perform unsafe casts.

**Identified Issues:**
1.  **Style Selector Composition (Issue #168):** Users cannot idiomatically combine `Part` and `State` enums (e.g., `Part::Indicator | State::Checked`) because they are scoped enums with different underlying types.
2.  **Callback Type Safety (Issue #170):** Callbacks in `Animation` (and potentially other subsystems) expose `void*` context pointers, requiring users to manually cast and wrap them into C++ `Object` instances.

**Root Cause:**
While `lvgl_cpp` successfully encapsulates LVGL's structural elements (Widgets, Objects), it falls short in **interoperability ergonomics** (combining enums) and **behavioral abstraction** (marshalling callback arguments).

---

## 2. Issue #168: Idiomatic Style Selectors

### 2.1 Problem Analysis

The `lvgl::Part` and `lvgl::State` enums are strictly typed `enum class` wrappers.
- `Part` wraps `lv_part_t` (uint32_t)
- `State` wraps `lv_state_t` (uint16_t)

LVGL's C API expects `lv_style_selector_t` which is semantically `part | state`.
Currently, users must write:
```cpp
// Current (Noisy, Unsafe)
obj.style(static_cast<lv_style_selector_t>(Part::Indicator) | 
          static_cast<lv_style_selector_t>(State::Checked))
   .bg_color(Color::Red);
```

### 2.2 Proposed Solution: Operator Overloading

The `Part | State` combination is the **only significant instance** in the LVGL API where two distinct enum types are designed to be bitwise-combined. Other bitmasks (like `ObjFlag` or `TextDecor`) combine values of the *same* type.

We will implement `operator|` overloads that seamlessly combine these types into a type-safe selector.

**Design:**

In `misc/enums.h`:
```cpp
// forward declaration
enum class Part : uint32_t;
enum class State : uint16_t;

// 1. Part | State
inline lv_style_selector_t operator|(Part p, State s) {
    return static_cast<lv_style_selector_t>(p) | static_cast<lv_style_selector_t>(s);
}

// 2. State | Part (Commutative)
inline lv_style_selector_t operator|(State s, Part p) {
    return static_cast<lv_style_selector_t>(p) | static_cast<lv_style_selector_t>(s);
}

// 3. Part | Part (mutually exclusive usually, but valid for ANY)
inline lv_style_selector_t operator|(Part p1, Part p2) {
    return static_cast<lv_style_selector_t>(p1) | static_cast<lv_style_selector_t>(p2);
}
```

This allows:
```cpp
// Proposed (Fluent)
obj.style(Part::Indicator | State::Checked).bg_color(Color::Red);
```

### 2.3 Impact
- **Ergonomics:** Removes casting noise.
- **Safety:** Ensures only valid enum combinations result in a selector.
- **Migration:** Non-breaking addition.

---

## 3. Issue #170: Typed Callbacks in Animation

### 3.1 Problem Analysis

The `Animation` class relies on `ExecCallback` defined as `std::function<void(void*, int32_t)>`.
When animating a C++ `Object`, the `void*` is the raw `lv_obj_t*`.
Users must manually wrap it:
```cpp
// Current (Friction)
anim.set_exec_cb([](void* var, int32_t v) {
    lvgl::Object obj(static_cast<lv_obj_t*>(var)); // Unmanaged wrapper
    obj.style().translate_y(v);
});
```

### 3.2 Proposed Solution: Typed Trampoline

We introduce a templated overload or specific `set_exec_cb` for `Object` types that performs the wrapping automatically.

**Design:**

In `misc/animation.h`:
```cpp
class Animation {
public:
    // ... existing ...

    /**
     * @brief Set a type-safe execution callback for Objects.
     * The callback receives a temporary reference to an unmanaged Object wrapper.
     */
    using ObjectExecCallback = std::function<void(Object&, int32_t)>;
    
    Animation& set_exec_cb(ObjectExecCallback cb) {
        // Capture user callback
        auto trampoline = [cb](void* var, int32_t val) {
            // Create temporary unmanaged wrapper
            Object obj(static_cast<lv_obj_t*>(var), Object::Ownership::Unmanaged);
            cb(obj, val);
        };
        return set_exec_cb(static_cast<ExecCallback>(trampoline));
    }
};
```

This allows:
```cpp
// Proposed (Fluent)
anim.set_exec_cb([](Object& obj, int32_t v) {
    obj.style().translate_y(v);
});
```

### 3.3 Implementation Details
- The existing `CallbackData` structure in `Animation` holds a `std::function<void(void*, int32_t)>`. The new method will simply wrap the user's typed lambda into this generic signature.
- **Lifecycle Safety:** The `Object` wrapper created in the trampoline is `Ownership::Unmanaged`. It is strict generic wrapper around the pointer. It is valid only for the duration of the callback, which is correct.

---

---

## 4. Systemic Review

This "marshall and wrap" pattern addresses the gap between C `void*` polymorphism and C++ strict typing.

**Other candidates:**
- **`InputDevice`**: If custom feedback callbacks are added, they should use `InputDevice&` not `lv_indev_t*`.
- **`Group`**: Focus callbacks should receive `Object&` focused (already identified in strategic plan).
- **`Event`**: The `Event` class already accesses the target via `get_target()`, which returns `Object*`. This is already consistent.

---

## 5. v2.1 Improvements: Fluent Refinement

Following critical review of the v2.0 implementation, several ergonomic refinements were identified:

### 5.1 Fluent Selector Chaining
While bitwise mixing (`Part::Knob | State::Pressed`) is supported, `StyleProxy` now supports explicit chaining for better discoverability:
```cpp
obj.style().part(Part::Knob).state(State::Pressed).bg_color(Color::Red);
```

### 5.2 Extended Part Operators
`Part` now supports the full suite of bitwise operators (`&`, `|`, `~`, etc.), treating it as a first-class maskable type consistent with `State`.

### 5.3 Callback Safety Guards
The `ObjectExecCallback` trampoline includes null-checks and documentation warnings to prevent misuse with non-widget variables in the animation system.

---

## 6. Next Steps
1.  Modify `misc/enums.h` to add `operator|` overloads.
2.  Modify `misc/animation.h` to add `set_exec_cb(ObjectExecCallback)`.
3.  Update `round_display_hello` example to use the new API to verify #170.
4.  Update a test case to verify style selector combination for #168.
