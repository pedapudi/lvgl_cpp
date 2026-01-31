# Scoped Enums Architecture: Type Safety & Discoverability

## 1. Executive Summary

This document describes the architectural transition in `lvgl_cpp` from C-style global enumerations and macros to C++ scoped enums (`enum class`). This shift addresses the core challenges of namespace pollution, weak type safety, and poor API discoverability inherent in large-scale C libraries like LVGL.

**Key Objectives:**
1.  **Eliminate Namespace Pollution**: Move widget-specific constants into class scopes.
2.  **Enforce Type Safety**: Prevent accidental mixing of unrelated enum types (e.g., passing a `Dir` where an `Align` is expected).
3.  **Enhance Discoverability**: Use IDE autocompletion to guide developers through valid options for a specific widget.
4.  **Maintain Bitmask Power**: Provide safe bitwise operators for combined flags and states.

---

## 2. Architecture of Enumerations

### 2.1 Global Scoped Enums
Fundamental types used across multiple subsystems are defined as scoped enums in `lvgl` namespace, typically located in `misc/enums.h`.

| C++ Scoped Enum | Underlying C Type | Description |
| :--- | :--- | :--- |
| `lvgl::Align` | `lv_align_t` | Object alignment options. |
| `lvgl::Part` | `lv_part_t` | Widget part selectors (Main, Knob, etc). |
| `lvgl::State` | `lv_state_t` | Interaction states (Pressed, Checked). |
| `lvgl::Dir` | `lv_dir_t` | Directions and orientations. |

### 2.2 Class-Scoped (Widget) Enums
Enums specific to a single widget are nested within that widget's class definition. This resolves the problem of global name collisions (e.g., every widget having a different "Normal" mode).

```cpp
namespace lvgl {
class Keyboard : public Widget {
public:
    enum class Mode : uint8_t {
        TextLower = LV_KEYBOARD_MODE_TEXT_LOWER,
        TextUpper = LV_KEYBOARD_MODE_TEXT_UPPER,
        // ...
    };
    
    Keyboard& set_mode(Mode mode); // Type-safe setter
};
}
```

---

## 3. Bitwise Operators & Masking

C++ `enum class` does not support bitwise operators by default. To maintain the ability to combine flags and states (e.g., `Part::Indicator | State::Checked`), `lvgl_cpp` implements explicit operator overloads.

### 3.1 Intramural Operations (Same Type)
For types like `Part` or `ObjFlag`, we provide standard OR/AND/NOT overloads to allow combining multiple values of the same type.

```cpp
inline Part operator|(Part lhs, Part rhs) {
    return static_cast<Part>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}
```

### 3.2 Inter-Enum Operations (Hybrid Types)
Unique to LVGL's style system, `Part` and `State` are often combined into a single `lv_style_selector_t`. `lvgl_cpp` provides "bridge" operators for these cases.

```cpp
// Allows: Part::Indicator | State::Pressed
inline lv_style_selector_t operator|(Part p, State s) {
    return static_cast<lv_style_selector_t>(p) | static_cast<lv_style_selector_t>(s);
}
```

---

## 4. Backward Compatibility Strategy

To ensure a smooth migration for existing projects, we employ a multi-tiered compatibility strategy.

### 4.1 Method Overloading
Setters provide two signatures: the modern scoped enum and the raw LVGL C-type.

```cpp
Keyboard& set_mode(Mode mode);              // Modern
Keyboard& set_mode(lv_keyboard_mode_t mode); // Compatibility
```

### 4.2 Deprecated Global Aliases
Old global enum names are retained but marked with `[[deprecated]]` to nudge developers toward the new class-scoped versions while maintaining build stability.

```cpp
// In misc/enums.h
enum class [[deprecated("Use Keyboard::Mode instead")]] KeyboardMode : uint8_t { ... };
```

---

## 5. Usage Comparison

### Before (C-Style / Global)
```cpp
lvgl::Keyboard kbd(screen);
kbd.set_mode(LV_KEYBOARD_MODE_TEXT_UPPER); // Raw MACRO
kbd.set_mode(lvgl::KeyboardMode::TextUpper); // Global enum (cluttered)
```

### After (Idiomatic C++)
```cpp
lvgl::Keyboard kbd(screen);
kbd.set_mode(lvgl::Keyboard::Mode::TextUpper); // Clean, scoped, discoverable
```

---

## 6. Implementation Checklist for New Widgets

When adding a new widget wrapper, follow these rules for enums:

1.  **Nesting**: Place enums inside the class if they are specific to that widget.
2.  **Naming**: Use `Mode`, `Type`, or `Orientation` as the enum name within the class.
3.  **Underlying Type**: Always specify the underlying type (e.g., `: uint8_t`) to match the LVGL C headers precisely.
4.  **Prefix**: Map enum values directly to LVGL constants (e.g., `Normal = LV_ARC_MODE_NORMAL`).
5.  **Dual Setters**: Implement both a scoped enum setter and a raw `lv_` type setter.
6.  **Getters**: Modernize getters to return the scoped enum type.

---

## 7. Future Evolution

As the library matures, we aim to:
*   **Remove Legacy Overloads**: In v1.0, deprecated global enums and raw C overloads will be removed.
*   **Automation**: Use scripts to audit that every `set_*` method in the library has a corresponding scoped enum overload.
*   **Stronger Typing**: Transition internal storage of modes to use the `enum class` directly rather than casting to the raw C-type at the earliest possible entry point.
