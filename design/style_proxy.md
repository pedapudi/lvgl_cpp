# StyleProxy Architecture

## 1. Overview

The `StyleProxy` pattern provides a modern, fluent C++ interface for modifying the local styles of LVGL objects. It replaces the legacy `Stylable` mixin, eliminating API bloat and improving type safety while maintaining zero runtime overhead.

## 2. Motivation

### 2.1. The Problem: "Monolithic Object" and API Bloat
In the previous design, the `Stylable` mixin injected over 100+ style-related methods (e.g., `set_style_bg_color`, `set_style_border_width`) directly into the `Widget` inheritance hierarchy. This caused:
- **Polluted Interface**: `lvgl::Button` had hundreds of methods, making autocompletion confusing.
- **Redundancy**: Stylable methods duplicated functionality found in the `Style` builder class.
- **Maintenance Cost**: Adding a new LVGL style property required updating multiple files.

### 2.2. The Solution: Transient Proxies
We removed the `Stylable` inheritance and introduced a factory method `style()` on the base `Object`. This method returns a temporary `StyleProxy` object that exposes the styling API only when requested.

## 3. Architecture

### 3.1. The `StyleProxy` Class
The `StyleProxy` is a lightweight wrapper around an `lv_obj_t*` and a `selector` (state/part). It contains no additional state and is intended to be stack-allocated and optimized away by the compiler.

```cpp
class StyleProxy {
 public:
  StyleProxy(lv_obj_t* obj, lv_style_selector_t selector)
      : obj_(obj), selector_(selector) {}

  // Fluent setters
  StyleProxy& bg_color(Color c);
  StyleProxy& radius(int32_t value);
  // ... (covers all LVGL style properties)

 private:
  lv_obj_t* obj_;
  lv_style_selector_t selector_;
};
```

### 3.2. Integration with `Object`
The `Object` class (and thus all Widgets) provides the entry point:

```cpp
class Object {
 public:
  // Returns a proxy for the default part/state
  StyleProxy style() { 
      return StyleProxy(raw(), LV_PART_MAIN); 
  }

  // Returns a proxy for a specific state or part
  StyleProxy style(lv_style_selector_t selector) { 
      return StyleProxy(raw(), selector); 
  }
};
```

## 4. User Experience

### 4.1. Fluent Chaining
Developers can chain multiple style settings in a single statement, similar to the Builder pattern.

```cpp
// New Syntax
btn.style()
    .bg_color(Palette::Blue)
    .radius(10)
    .border_width(2)
    .border_color(Palette::White);
```

### 4.2. State and Part Handling
Targeting specific states or parts is intuitive and explicit.

```cpp
// Set styles for the Pressed state
btn.style(State::Pressed)
    .bg_color(Palette::DarkBlue);

// Set styles for the Scrollbar part
btn.style(Part::Scrollbar)
    .bg_opa(Opacity::Transparent);
```

### 4.3. Type Safety
The API utilizes C++ Scoped Enums (e.g., `lvgl::State`, `lvgl::Part`, `lvgl::Color`) to prevent magic numbers and type mismatches.

## 5. Migration Guide

| Feature | Legacy Syntax (Removed) | New Syntax (StyleProxy) |
| :--- | :--- | :--- |
| **Simple Prop** | `btn.set_style_bg_color(Color, 0);` | `btn.style().bg_color(Color);` |
| **With State** | `btn.set_style_bg_color(C, LV_STATE_PRESSED);` | `btn.style(State::Pressed).bg_color(C);` |
| **With Part** | `btn.set_style_bg_color(C, LV_PART_KNOB);` | `btn.style(Part::Knob).bg_color(C);` |

## 6. Performance
The `StyleProxy` is a zero-cost abstraction. 
- **Allocation**: Stack-allocated (transient).
- **Inlining**: Methods are inline, allowing the compiler to generate direct calls to `lv_obj_set_style_*` functions.
- **Size**: `sizeof(StyleProxy)` is minimal (2 pointers/integers), fitting in registers.

## 7. Future Work
- Complete coverage of all 100+ LVGL style properties in `StyleProxy`.
- Implement similar proxies for **Layout** (`obj.layout().flex(...)`) and **Scroll** (`obj.scroll().to(...)`) to fully clean up the `Object` interface.
