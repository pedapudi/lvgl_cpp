# lvgl_cpp: A Modern C++20 Interface for LVGL 9

> **Transforming the LVGL C library into a type-safe, fluent, and highly performant C++ framework.**

`lvgl_cpp` transforms the [LVGL](https://lvgl.io/) C library into a distinctively modern C++ framework. It provides **RAII memory management**, **functional event callbacks**, and a **fluent API** while strictly maintaining the performance characteristics of the underlying C library suitable for high-performance embedded systems.

---

## Why lvgl_cpp?

The C++ wrapper eliminates common C-style pitfalls by providing structured safety and modern developer ergonomics.

| Feature | Raw C API (`lvgl`) | C++ Wrapper (`lvgl_cpp`) |
| :--- | :--- | :--- |
| **Memory** | Manual `lv_obj_del` and `free`. Potentially leak-prone. | **RAII**. Automatic destructor cleanup through clear ownership. |
| **Typing** | `void*` context pointers and unsafe record casting. | **Strongly Typed**. Distinct classes for every widget. |
| **Events** | `void event_cb(lv_event_t*)` with complex switch blocks. | **Lambdas**: `btn.on_clicked([](Event e) { ... })`. |
| **Styles** | Verbose: `lv_style_set_bg_color(&style, ...)` | **Fluent**: `style().bg_color(Red).radius(8)` |
| **Safety** | "Monolithic Object" `lv_obj_t` exposes all methods. | **CRTP Mixins**. Only valid methods are exposed per widget. |

---

## Installation and Setup

`lvgl_cpp` is designed to be integrated as a subdirectory in your CMake project alongside the main `lvgl` repository.

### Prerequisites

*   **C++ Standard**: C++20 or later.
*   **LVGL Version**: v9.0 or later.
*   **Build System**: CMake 3.10 or later (or ESP-IDF).

### Integration

1.  Clone `lvgl` and `lvgl_cpp` into your project components:
    ```bash
    git submodule add https://github.com/lvgl/lvgl.git components/lvgl
    git submodule add https://github.com/pedapudi/lvgl_cpp.git components/lvgl_cpp
    ```

2.  Add to your `CMakeLists.txt`:
    ```cmake
    # Add LVGL and the C++ Wrapper
    add_subdirectory(components/lvgl)
    add_subdirectory(components/lvgl_cpp)

    target_link_libraries(${PROJECT_NAME} PRIVATE lvgl_cpp)
    ```

---

## Quick Start: Hello World

Here is a complete example of a modern, styled interface built in just a few lines of code.

```cpp
#include "lvgl_cpp.h"

using namespace lvgl;

void build_ui() {
    // 1. Get the active screen wrapper
    auto screen = Screen::active();

    // 2. Create a shared style using the fluent builder
    static Style main_style = Style()
        .bg_color(Palette::Blue)
        .radius(12)
        .border_width(2)
        .border_color(Palette::Cyan)
        .shadow_width(15);

    // 3. Create and style a Button
    Button btn(screen);
    btn.set_size(160, 60)
       .center()
       .add_style(main_style);

    // 4. Add a Label to the Button
    Label label(btn);
    label.set_text("Launch")
         .center();

    // 5. Modern Event Handling with Lambdas
    btn.on_clicked([](Event e) {
        // Safe access to the specific widget type
        auto target = e.get_target<Button>(); 
        target.style().bg_color(Palette::Green);
        printf("System Launched!\n");
    });
}

// ⚠️ Note on Lambda Capture
// Always prefer e.get_target<T>() over capturing local wrappers by reference 
// (e.g., [&btn]). C++ wrappers are lightweight views; capturing them by 
// reference to a local scope can result in dangling references when the 
// event eventually fires.
```

---

## Architecture: Understanding the Bridge

### 1. Ownership Model and RAII
The library prevents memory leaks by defining strict ownership policies when C++ wrappers are created.

| Policy | Enum Value | Result | Common Use |
| :--- | :--- | :--- | :--- |
| **Managed** | `Ownership::Managed` | Destructor calls `lv_obj_del`. | New widget creation: `Button(parent)`. |
| **Unmanaged** | `Ownership::Unmanaged` | Destructor does nothing to C object. | Views or pointers: `Screen::active()`. |

### 2. Widget Hierarchy and Composition 
We solve the "Monolithic Object" problem using a **CRTP (Curiously Recurring Template Pattern)**. 

Functionality is not piled into a single base class. Instead, it is composed via specialized **Systemic Proxies**, ensuring `lvgl::Object` remains lightweight:

*   **`style()`**: Returns a `StyleProxy` for fluent local styling.
*   **`scroll()`**: Returns a `ScrollProxy` for scrolling and snapping.
*   **`layout()`**: Returns a `LayoutProxy` for Flexbox and Grid configuration.
*   **`interaction()`**: Returns an `InteractionProxy` for gestures and input groups.
*   **`tree()`**: Returns a `TreeProxy` for DOM-like traversal (children and siblings).

### 3. Hardware Excellence
`lvgl_cpp` includes specialized optimizations for high-end embedded hardware, particularly for the **ESP32-S3**:

- **SIMD Acceleration**: XTensa-optimized assembly shims for 2x to 3x faster color blending and image rendering on S3 chips.
- **Event-Driven Task Loop**: A replacement for the standard polling loop that uses Task Notifications for sub-millisecond UI responsiveness and lower CPU idle usage.
- **DMA Awareness**: Custom RAII deallocators in `DrawBuf` ensure that display buffers are correctly aligned and allocated in DMA-capable memory.

---

## Documentation and Roadmaps

For detailed technical analysis and future plans, refer to the following resources:

*   **[Technical Roadmap](design/ROADMAP_V2.md)**: Current status and the path to v1.0.
*   **[API Coverage Report](design/api_coverage_plan.md)**: Tracking coverage against the 1,800+ LVGL functions.
*   **[Performance and Memory Study](design/memory_analysis.md)**: Analysis showing a minimal ~48-byte overhead per wrapper.
*   **[Hardware Configuration Guide](design/esp32_hardware_tuning.md)**: Optimizing for Big-Endian SPI and RGB displays.

License: [Apache-2.0](LICENSE)
