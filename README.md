# LVGL C++ Wrapper (lvgl_cpp)

A modern, robust C++ wrapper for [LVGL (Light and Versatile Graphics Library)](https://lvgl.io/). This library provides a native C++ interface with RAII-style memory management, type-safe widgets, and functional event callbacks.

## Features

*   **RAII Memory Management**: Widgets are automatically deleted when their parent is destroyed. Explicit ownership semantics ensure safety.
*   **Type-Safe Wrappers**: Dedicated classes for LVGL widgets (`Button`, `Label`, `Slider`, etc.) inheriting from a base `Object`.
*   **C++ API**: Use C++ patterns like `return Object` instead of `return lv_obj_t*`.
*   **Functional Events**: transparently use `std::function` and lambdas for LVGL events.
*   **Style Classes**: C++ wrapper for `lv_style_t`.
*   **Observer Pattern**: C++ implementation of the LVGL property-observer system.

## Installation

### Prerequisites
*   LVGL v9.x
*   CMake 3.10+
*   C++17 compliant compiler

### Integration
1.  Add `lvgl_cpp` to your project structure (alongside `lvgl`).
2.  In your `CMakeLists.txt`:

```cmake
add_subdirectory(lvgl)      # Build LVGL Core
add_subdirectory(lvgl_cpp)  # Build C++ Wrapper

add_executable(my_app main.cc)
target_link_libraries(my_app PRIVATE lvgl_cpp) # Links lvgl automatically
```

### Directory Structure
```
project/
├── CMakeLists.txt
├── main.cc
├── lvgl/           # Official LVGL repository
└── lvgl_cpp/       # This library
```

## Getting Started

### Initialization
Initialize LVGL and create a display driver as usual (C API is still useful for system init).

```cpp
#include "lvgl.h"
#include "lvgl_cpp/core/object.h"
#include "lvgl_cpp/widgets/button.h"
#include "lvgl_cpp/widgets/label.h"

int main() {
    lv_init();
    lv_display_create(800, 480); // Create default display

    // Create a screen (Object with no parent)
    lvgl::Object screen; 
    
    // Create a button on the screen
    lvgl::Button btn(&screen);
    btn.set_size(100, 50);
    btn.center();
    
    // Create a label on the button
    lvgl::Label label(&btn);
    label.set_text("Click Me");
    label.center();

    // Event handling
    btn.add_event_cb(LV_EVENT_CLICKED, [](lv_event_t* e) {
        printf("Button clicked!\n");
    });

    while(1) {
        lv_timer_handler();
        usleep(5000);
    }
}
```

## Architecture & Memory Management

The wrapper tracks ownership to prevent memory leaks and double-frees.

### Ownership Model
1.  **Owned Objects** (`owning = true`):
    *   Created when you construct a widget with a parent: `Button btn(&screen);`
    *   The C++ wrapper *owns* the underlying `lv_obj_t`.
    *   When the C++ object goes out of scope, it **deletes** the LVGL object.
    *   *Usage*: Valid for local/stack objects or `std::unique_ptr` member variables.
    
2.  **Non-Owned Objects** (Wrappers/Proxies):
    *   Created when you wrap an existing `lv_obj_t*`: `Object obj(ptr);`
    *   Also created when methods return objects: `Object tab = tabview.add_tab("Tab");`
    *   The C++ wrapper acts as a **reference**.
    *   Destruction of the C++ wrapper **does NOT** delete the LVGL object.

### The Problem of "Parent Deletion" vs "RAII"
LVGL deletes children when a parent is deleted. The C++ wrapper handles this gracefully.
*   If a parent `screen` is deleted, it deletes its children in LVGL logic.
*   If you hold a C++ `Button btn(&screen)` wrapper, and `screen` is deleted first, `btn`'s underlying object is destroyed.
*   The `Object` class installs an `LV_EVENT_DELETE` hook. When the underlying object is deleted (by parent), the C++ wrapper marks its internal pointer as `nullptr`.
*   When the `btn` wrapper destructor runs later, it sees `nullptr` and does nothing. **Safe.**

## Usage Guide

### Widgets
All core widgets are supported in `lvgl_cpp/widgets/`.
*   Standard naming: `lv_button_create` -> `lvgl::Button`.
*   Constructors:
    *   `Button()`: Create with default parent (active screen).
    *   `Button(Object* parent)`: Create as child of parent.
    *   `Button(lv_obj_t* obj)`: Wrap existing object.

### Return Types
Methods that create children return specific C++ wrappers by value (move semantics) rather than generic `Object`s.
```cpp
// TabView::add_tab returns a TabPage wrapper
lvgl::TabPage tab = tabview.add_tab("Settings");

// Menu::page_create returns a MenuPage wrapper
lvgl::MenuPage page = menu.page_create("Main");

// TileView::add_tile returns a Tile wrapper
lvgl::Tile tile = tileview.add_tile(0, 0, LV_DIR_BOTTOM);

// List::add_button returns a Button wrapper
lvgl::Button btn = list.add_button(LV_SYMBOL_OK, "Apply");
```

### Events
Use safe C++ lambdas. Closures are automatically managed.
```cpp
btn.add_event_cb(LV_EVENT_CLICKED, [=](lv_event_t* e) {
    auto code = lv_event_get_code(e);
    // ... logic
});
```

### Timers
Use the `lvgl::Timer` class for periodic tasks.
```cpp
#include "lvgl_cpp/misc/timer.h"

// Create a timer that runs every 500ms
lvgl::Timer timer(500, [](lvgl::Timer* t) {
    printf("Tick!\n");
});
```

### Animation
Use the `lvgl::Animation` builder for complex animations.
```cpp
#include "lvgl_cpp/misc/animation.h"

lvgl::Animation anim;
anim.set_var(btn.raw())
    .set_values(0, 100)
    .set_duration(1000)
    .set_exec_cb([](void* var, int32_t v) {
        lv_obj_set_x((lv_obj_t*)var, v);
    })
    .start();
```

### Styles
```cpp
#include "lvgl_cpp/core/style.h"

lvgl::Style style;
style.init();
style.set_bg_color(lv_palette_main(LV_PALETTE_BLUE));
style.set_radius(10);

btn.add_style(style, LV_PART_MAIN);
```

## Observer Pattern

LVGL C++ provides a robust wrapper around the Observer pattern.

### Subjects
Subjects hold state (Int, String, Pointer, Color) and notify observers on change.
```cpp
lvgl::IntSubject speed(0);
speed.set(100);
```

### Widget Binding
Widgets can bind their properties to a Subject. The binding returns an `Observer` object that enforces RAII (Resource Acquisition Is Initialization). **You must keep the Observer object alive** to maintain the binding.

```cpp
lvgl::Slider slider(lv_screen_active());
// The binding persists only as long as 'obs' exists.
lvgl::Observer obs = slider.bind_value(speed);

// If you ignore the return value, the observer is destroyed immediately!
// [[nodiscard]] will warn you about this.
// slider.bind_value(speed); // WARNING: Binding immediately removed
```

### Manual Observers
You can add custom observers to any subject.
```cpp
auto obs = speed.add_observer([](lv_observer_t*, lv_subject_t* s) {
    printf("Speed changed: %d\n", lv_subject_get_int(s));
}, nullptr);
```

### Memory Management
- **Subjects**: Must outlive their Observers.
- **Observers**: When an `Observer` C++ object goes out of scope, it automatically unsubscribes from the subject (if it owns the subscription).

### Helper Utilities
- **FileSystem**: `lvgl_cpp/misc/file_system.h` provides `File` and `Directory` classes.
- **Color**: `lvgl_cpp/misc/color.h` provides a `Color` class with mixing/darkening/lightening support.

## API Reference
The API is fully documented in the header files.
- **Core**: `lvgl_cpp/core/` (Object, Group, Style, Observer)
- **Widgets**: `lvgl_cpp/widgets/` (All widgets)
- **Misc**: `lvgl_cpp/misc/` (Timer, Animation, FS, Color)

## Contributing
The library is structured into `core` (Object, Style, Observer) and `widgets`.
To add a new widget:
1.  Create `widgets/my_widget.h` inheriting `Object`.
2.  Implement constructors: `MyWidget()`, `MyWidget(Object*)`, `MyWidget(lv_obj_t*)`.
3.  Wrap specific API methods.

