# LVGL C++ Wrapper (`lvgl_cpp`)

> **A Modern, Type-Safe, Zero-Overhead C++20 Interface for LVGL 9**

`lvgl_cpp` transforms the [LVGL](https://lvgl.io/) C library into a distinctively modern C++ framework. It provides **RAII memory management**, **functional event callbacks**, and a **fluent API** while strictly maintaining the performance characteristics of the underlying C library suitable for embedded systems.

---

## 🚀 Why `lvgl_cpp`?

| Feature | Raw C API (`lvgl`) | C++ Wrapper (`lvgl_cpp`) |
| :--- | :--- | :--- |
| **Memory** | Manual `lv_obj_del` / `free`. Prone to leaks. | **RAII**. Automatic destructor cleanup. |
| **Typing** | `void*` context pointers. Unsafe casts. | **Strongly Typed**. `Button`, `Label`, `Slider` classes. |
| **Events** | `void event_cb(lv_event_t*)` with switches. | **Lambdas**: `btn.add_event_cb(..., [=](Event e) { ... })`. |
| **Styles** | Verbose: `lv_style_set_bg_color(&style, ...)` | **Fluent**: `style().bg_color(Red).transform_scale(256)` |
| **Safety** | "Monolithic Object" `lv_obj_t` has all API methods. | **CRTP Mixins**. Only exposing valid methods for each widget. |

---

## 📦 Installation

`lvgl_cpp` is designed to be added as a subdirectory in your CMake project alongside the main `lvgl` repository.

### Prerequisites

*   **C++ Standard**: C++20 or later.
*   **LVGL Version**: v9.0+.
*   **Build System**: CMake 3.10+.

### Integration

1.  Clone `lvgl` and `lvgl_cpp` into your project:
    ```bash
    git submodule add https://github.com/lvgl/lvgl.git components/lvgl
    git submodule add https://github.com/pedapudi/lvgl_cpp.git components/lvgl_cpp
    ```

2.  Add to your `CMakeLists.txt`:
    ```cmake
    cmake_minimum_required(VERSION 3.10)
    project(my_gui_app)

    set(CMAKE_CXX_STANDARD 20)

    # Add LVGL and the C++ Wrapper
    add_subdirectory(components/lvgl)
    add_subdirectory(components/lvgl_cpp)

    add_executable(main main.cpp)
    target_link_libraries(main PRIVATE lvgl_cpp)
    ```

---

## 🏁 Getting Started

Here is a complete example showing the "Hello World" of `lvgl_cpp` with a styled button and event handling.

```cpp
#include <thread>
#include <chrono>
#include "lvgl.h"
#include "lvgl_cpp/core/object.h"
#include "lvgl_cpp/display/display.h"
#include "lvgl_cpp/widgets/button.h"
#include "lvgl_cpp/widgets/label.h"
#include "lvgl_cpp/widgets/screen.h"
#include "lvgl_cpp/misc/style.h"

using namespace lvgl;

void create_ui(Screen& screen) {
    // 1. Create a Style using the Fluent Builder
    static Style btn_style; // Static to keep it alive (or use shared_ptr)
    btn_style.init()
             .bg_color(Palette::Blue)
             .radius(8)
             .border_width(2)
             .border_color(Palette::Cyan)
             .shadow_width(10);

    // 2. Create a Button
    Button btn(&screen);
    btn.set_size(150, 60)
       .center()
       .add_style(btn_style) // Shared style
       .style().bg_color(Palette::Red); // Local style override
    btn.add_flag(ObjFlag::Checkable); // Type-safe flags

    // 3. Add a Label to the Button
    Label label(&btn);
    label.set_text("Click Me!")
         .center();

    // 4. Handle Events with C++ Lambdas
    btn.add_event_cb(EventCode::Clicked, [](Event event) {
        // 'event' is a C++ wrapper around lv_event_t
        auto target_btn = Button(event.get_target(), Ownership::Unmanaged);
        
        if (target_btn.has_state(State::Checked)) {
            printf("Button Checked!\n");
        } else {
            printf("Button Unchecked!\n");
        }
    });
}

int main() {
    lv_init();
    
    // Create default display using the C++ wrapper
    auto disp = Display::create(800, 480);
    
    // Get the active screen wrapper
    auto screen = Screen::active();
    
    create_ui(screen);

    while (true) {
        lv_timer_handler();
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

---

## 🧠 Architecture & Concepts

### 1. Ownership Model (RAII)

The library prevents memory leaks by strictly defining who owns the LVGL object.

| Policy | Enum Value | Description | Use Case |
| :--- | :--- | :--- | :--- |
| **Managed** | `Ownership::Managed` | The C++ object **owns** the underlying `lv_obj_t`. When the C++ object destructor runs, it calls `lv_obj_del`. | Creating new widgets locally (e.g., `Button btn(&screen)`). |
| **Unmanaged** | `Ownership::Unmanaged` | The C++ object is a **weak view**. Destruction does NOT delete the LVGL object. | Wrapping existing pointers (e.g., `screen`, `event.get_target()`). |
| **Default** | `Ownership::Default` | Auto-detects based on constructor. | `Button(&parent)` → **Managed**. `Button(ptr)` → **Unmanaged**. |

### 2. Widget Type Hierarchy

We avoid the "Monolithic Object" anti-pattern. `lvgl::Object` is the base, but functionality is composed via **CRTP Mixins**:

*   **`Positionable<T>`**: `set_x`, `set_y`, `align`, `center`, `set_size`...
*   **`Scrollable<T>`**: `scroll_to`, `set_scrollbar_mode`...
*   **Generic Styling**: `obj.style()` returns a **StyleProxy** for fluent local styling (`bg_color`, `border_width`...) without bloating the main class.

This means `lvgl::Object` only exposes generic methods. To use button-specific features, you must use `lvgl::Button`.

### 3. The Style Builder

LVGL v9 styles are powerful but verbose in C. `lvgl_cpp` simplifies this with a fluent builder:

```cpp
// Create and initialize in one chain
Style style = Style()
    .bg_color(Color(0xFF0000))
    .bg_opa(Opacity::Cover)
    .text_font(&lv_font_montserrat_14)
    .pad_all(10)
    .transition(&trans_desc);
```

### 4. Input & Groups

Full support for Encoder and Keypad navigation through `lvgl::Group`:

```cpp
lvgl::Group group = lvgl::Group::get_default();
group.add_obj(btn1);
group.add_obj(btn2);
```

---

## 📚 Documentation

The `design/` directory contains detailed architectural decision records (ADRs) and plans:

*   **[Roadmap](design/ROADMAP_V2.md)**: Current status and future phases.
*   **[Strategic Plan](design/strategic_improvement_plan.md)**: Deep dive into API coverage goals.
*   **[Memory Analysis](design/memory_analysis.md)**: Performance overhead study.
*   **[Widget Standardization](design/issue_61_standardization.md)**: Constructor patterns.

---

## 🛠️ Contributing

1.  **Code Style**: 2-space indentation, Google C++ Style Guide.
2.  **Naming**: `snake_case` for methods/variables, `PascalCase` for classes.
3.  **Tests**: All new features must include unit tests in `tests/`.

License: [MIT](LICENSE)
