# LVGL C++ API Modernization Design Document

## 1. Executive Summary

The current `lvgl_cpp` library functions primarily as a thin wrapper around the LVGL C API. While it provides basic RAII for object lifetime, it lacks the expressive power, type safety, and ease of use expected from modern C++ libraries. Developers often find themselves dropping down to C functions to accomplish standard tasks, negating the value of the wrapper.

This design document proposes a fundamental re-architecture of `lvgl_cpp` to become a **Fluent, Type-Safe, and Idiomatic C++20 Framework**. The goal is to make the "Pit of Success" wide—making correct usage easy and incorrect usage difficult.

## 2. Design Principles

Our design philosophy is grounded in resolving the friction between LVGL's flexible C architecture and modern C++ development patterns.

### 2.1. Object-Oriented Composition vs. Monolithic Inheritance
*   **Analysis of LVGL (C)**: In LVGL's C API (`lv_obj_class.h`), all widgets inherit from a "Base Object" (`lv_obj_t`). This base object carries a massive amount of state: coordinates, styles for every part, flags, and event lists. The C API functions (`lv_obj_set_x`, `lv_obj_add_style`) operate on `lv_obj_t*`, effectively treating every widget as a monolithic entity.
*   **The Problem**: A naive C++ wrapper (`class Object`) that exposes all 80+ `lv_obj_...` functions creates a "God Object." Subclasses like `Button` or `Label` inherit methods that may not be semantically relevant (e.g., `set_scrollbar_mode` on a simple icon), polluting the API surface.
*   **The Solution**: **Mixins (Traits)**. We will use C++20 concepts and CRTP to decompose attributes.
    *   **Before**: `lv_obj_set_x(btn, 10)`, `lv_obj_set_width(btn, 100)`.
    *   **After**: `Object` contains only lifetime and tree management. `Widget<T>` brings in `Positionable<T>`, `Sizable<T>`, `Stylable<T>`.

### 2.2. Type-Safe Styles
*   **Analysis of LVGL (C)**: Styles (`lv_style_t`) are untyped bags of properties accessed via `lv_style_set_prop(style, prop_id, value)`. The C API relies on runtime checks or programmer discipline to match `prop_id` (e.g., `LV_STYLE_BG_COLOR`) with the correct value type (`lv_color_t`).
*   **The Problem**: It's easy to pass an integer where a color is expected, or mix up scalar values.
*   **The Solution**: **Strongly Typed Builders**.
    *   **Before**:
        ```c
        static lv_style_t style;
        lv_style_init(&style);
        lv_style_set_bg_color(&style, lv_palette_main(LV_PALETTE_BLUE));
        lv_style_set_bg_opa(&style, LV_OPA_50);
        ```
    *   **After**:
        ```cpp
        auto style = Style()
            .bg_color(Palette::Blue)
            .bg_opa(Opacity::Cover)
            .border_width(2_px);
        ```

### 2.3. Event Safety
*   **Analysis of LVGL (C)**: Events (`lv_event_t`) use `void*` for parameters (`lv_event_get_param`) and user data. Callbacks are function pointers that lose context.
*   **The Problem**: `void_ptr` casting is error-prone. Capturing `this` or local state in C callbacks requires manual boilerplate structure handling.
*   **The Solution**: **std::function and Event Wrappers**.
    *   **Before**:
        ```c
        static void my_cb(lv_event_t * e) {
             MyData* data = (MyData*)lv_event_get_user_data(e);
             // unsafe cast
        }
        lv_obj_add_event_cb(btn, my_cb, LV_EVENT_CLICKED, &my_data);
        ```
    *   **After**:
        ```cpp
        btn.on_click([this](Event e) {
            this->handle_click(); // Safe capture
            auto code = e.code(); // Typed enum
        });
        ```

### 2.4. Espressif & Embedded First
*   **Use Case**: Developing for ESP32 requires strict memory management and clear hardware integration. Standard ESP-IDF examples often mix hardware init code with UI logic, leading to "spaghetti code."
*   **Design goal**: The C++ API must be **Zero-Overhead** in hot paths and allocate safely.
    *   **Allocation**: Use standard smart pointers (`std::unique_ptr`) for custom wrapper data, but rely on LVGL's memory pool for widgets to stay compatible with ESP-IDF's `esp_lvgl_port`.
    *   **Hardware Abstraction**: Utilities to easily bind physical buttons (GPIO) to LVGL Groups, mirroring `esp_iot_solution` patterns but with C++ type safety.

---

## 3. Core Architecture

### 3.1. The Mixin Pattern (CRTP)

Currently, `Object` contains every possible method (Positioning, Sizing, Styling, Flags). This makes it bloated and hard to maintain. Furthermore, subclasses like `Button` have to manually shadow methods to support fluent chaining.

We will use the **Curiously Recurring Template Pattern (CRTP)** to implement Mixins. This allows us to define behavior once and inject it into any Widget while preserving the return type for method chaining.

```cpp
// Example Concept
template <typename Derived>
struct Positionable {
    Derived& set_x(int32_t x) {
        lv_obj_set_x(static_cast<Derived*>(this)->raw(), x);
        return *static_cast<Derived*>(this);
    }
    // ... set_y, set_pos, align ...
};

template <typename Derived>
struct Sizable {
    Derived& set_width(int32_t w) {
        lv_obj_set_width(static_cast<Derived*>(this)->raw(), w);
        return *static_cast<Derived*>(this);
    }
    // ... set_height, set_size ...
};

class Button : public Object, 
               public Positionable<Button>, 
               public Sizable<Button> {
    // ...
};

// Usage
btn.set_x(10).set_width(100); // Returns Button&, not Object& or Positionable&
```

### 3.2. Widget Base Class

We will introduce a `Widget<T>` base class that inherits from `Object` and aggregates standard mixins.

```cpp
template <typename Derived>
class Widget : public Object,
               public Positionable<Derived>,
               public Sizable<Derived>,
               public Stylable<Derived>,
               public EventSource<Derived> {
    // Common widget logic
};

class Button : public Widget<Button> {
    // Button-specific logic
};
```

### 3.3. Smart Properties

Instead of just `get_width()` and `set_width()`, we can explore a property proxy for cleaner syntax if desired, or stick to the fluent setters. Given the embedded constraints, we will stick to **Fluent Getters/Setters** as the primary interface but ensure they are consistent.

*   `width(int32_t)` -> sets width, returns `Derived&`
*   `width()` -> returns `int32_t`

## 4. Subsystems Redesign

### 4.1. Style System

The current `Style` class is too empty. We need a comprehensive wrapper.

**Proposal**: A `Style` builder that works independent of objects.

```cpp
Style primary_btn_style;
primary_btn_style
    .bg_color(Palette::Blue)
    .radius(8)
    .border_width(2)
    .border_color(Palette::White);

btn.add_style(primary_btn_style);
```

And for local styles (direct on object):

```cpp
btn.style()  // Returns a temporary StyleProxy<Button>
    .bg_color(Palette::Red)
    .opa(50);
```

### 4.2. Layouts

Layouts in LVGL (Flex/Grid) are powerful but verbose in C. We will create helper classes.

**Flex Helpers**:
```cpp
// Instead of: obj.set_flex_flow(LV_FLEX_FLOW_ROW);
obj.set_layout(Flex::Row().gap(10).align_center());
```

**Grid Helpers**:
```cpp
// Declarative Grid
static const auto col_dsc = Grid::Track({80, Grid::Fr(1), 80});
static const auto row_dsc = Grid::Track({30, 30});
obj.set_layout(Grid(col_dsc, row_dsc));

child.grid_cell(0, 0); // col 0, row 0
```

### 4.3. Events

We will enforce the use of `lvgl::Event` wrapper.

```cpp
btn.on_click([](Event e) {
    auto target = e.target<Button>(); // Type-safe cast wrapper
    target.set_text("Clicked!");
});
```

We will deprecate `add_event_cb` taking raw `lv_event_code_t` in favor of semantic helpers:
*   `on_click(...)`
*   `on_press(...)`
*   `on_value_change(...)`

## 5. Utility Functions & Higher Level Abstractions

To bridge the usability gap, we will add a `utils` namespace or static helper classes.

### 5.1. Alerts and Dialogs
`msgbox.h` exists, but we can make it simpler.

```cpp
Alert::show("Title", "Message", {"OK", "Cancel"}, [](int button_idx) {
    // Handle result
});
```

### 5.2. Timers
Type-safe generic timers.

```cpp
auto timer = Timer::create(100ms, []() {
    // periodic task
});
```

### 5.3. Animations
A fluent animation builder.

```cpp
Anim::on(btn)
    .prop(Anim::X, 0, 100)
    .duration(500ms)
    .ease(Anim::Ease::OutCubic)
    .start();
```

### 5.4. ESP32 Concrete Use Case

**Scenario**: A WiFi Configuration Screen on an ESP32-S3 with a generic 3.5" TFT.

**Before (Raw C/Current Wrapper)**
```cpp
// Verbose, disjointed logic
lv_obj_t* btn = lv_button_create(lv_screen_active());
lv_obj_set_size(btn, 120, 40);
lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);
lv_obj_add_event_cb(btn, my_handler, LV_EVENT_CLICKED, NULL);

lv_obj_t* label = lv_label_create(btn);
lv_label_set_text(label, "Scan WiFi");
lv_obj_center(label);
```

**After (New API)**
```cpp
// Compositional, Readable, Safe
Button(Screen::active())
    .size(120, 40)
    .center()
    .style(Style().bg_color(Palette::Blue).radius(8))
    .label("Scan WiFi") // Helper to auto-create internal label
    .on_click([this](auto) {
        this->scan_wifi_async();
    });
```
This reduces cognitive load, allowing the developer to focus on the *logic* (Scanning WiFi) rather than the *mechanics* (creating labels, aligning them, casting pointers).

---

## 6. Implementation Plan

This is a major refactor. We will execute it in phases to maintain stability.

### Phase 1: Foundation (The Mixins)
1.  Create `core/mixins/` directory.
2.  Implement `Positionable<T>`, `Sizable<T>`, `Clickable<T>`, `Stylable<T>`.
3.  Create `Widget<T>` base template.
4.  **Verification**: Port `Button` to use `Widget<Button>` and verify strict backward compatibility (or minimal breaking changes).

### Phase 2: Core Refactor
1.  Strip `Object` class down to the bare essentials (wrapper around `lv_obj_t`, RAII logic).
2.  Move extensive methods from `Object` into respective Mixins.
3.  Implement `Style` builder class with comprehensive property coverage.
4.  **Verification**: Update `Label`, `Container` (new) to use the new system.

### Phase 3: The Big Migration
1.  Systematically update all 60+ widgets in `widgets/` to inherit from `Widget<T>`.
2.  Remove manual "shadow" methods (e.g. `set_width` in `Button`) as Mixins now handle return types.
3.  Update all examples and tests.

### Phase 4: High-Level Utilities
1.  Implement `Layout` helpers (Flex/Grid).
2.  Implement `Anim` builder.
3.  Implement `Timer` wrappers.
4.  Create new "Modern" examples showing off the new syntax.

## 7. Migration Guide
We will provide a `MIGRATION.md` for users.
*   `obj.set_width(10)` -> `obj.width(10)` (Preferred, though setter might remain as alias).
*   `lv_style_set_bg_color(&style, ...)` -> `style.bg_color(...)`.

---

## 8. Summary of changes

| Feature | Old `lvgl_cpp` | New `lvgl_cpp` |
| :--- | :--- | :--- |
| **Chaining** | Limited / Manual | Pervasive (CRTP) |
| **Styles** | Manual `lv_style_set_...` wrappers | Fluent `Style` builder |
| **Object** | Monolithic class | Composable Mixins |
| **Events** | `add_event_cb` with defines | `on_click`, `on_change` helpers |
| **Layouts** | Raw enums | `Flex::Row()`, `Grid::Builder` |
| **Embedded**| Mixed init/logic | Clean HAL separation |
