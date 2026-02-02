# Migration Guide: Moving to lvgl_cpp v0.2.0

## Overview
As part of the modernization of the C++ wrapper for LVGL 9, we have introduced a new architecture based on Mixins and the `Widget<T>` pattern. This change improves type safety, reduces code duplication, and enables a more fluent API.

## Key changes

### 1. `Object` is no longer a "Monolithic Base Class"
In previous versions, the base `Object` class contained methods for positioning, sizing, and alignment (`set_x`, `set_width`, `align`, etc.). These have been removed to enforce usage of concrete Widget types.

**Old Code:**
```cpp
lvgl::Object obj(parent);
obj.set_x(10); // Compilation Error
obj.set_size(100, 100); // Compilation Error
```

**New Code:**
```cpp
// Use a concrete widget, or cast if necessary (though rare for generic objects)
lvgl::Label label(parent);
label.set_x(10);
label.set_size(100, 100);

// For a generic container, use lvgl::Widget<lvgl::Object> isn't standard, 
// usually you want a specific widget like lvgl::Button or lvgl::Container (if it existed)
// or simply keep using specific widgets.
```

### 2. Constructors
Constructors now prefer generic `Object& parent` referencing or `lv_screen_active()` for defaults.

**Old Code:**
```cpp
lvgl::Button btn(nullptr); // Unsafe if unchecked
```

**New Code:**
```cpp
lvgl::Button btn; // Automatically uses lv_screen_active()
// OR
lvgl::Button btn(parent_obj);
```

### 3. Fluency and Typed Properties
Most setters now return `*this` typed to the specific class, allowing chaining. Additionally, raw C constants (macros) are being phased out in favor of C++ scoped enums.

```cpp
lvgl::Button btn;
btn.set_size(100, 50)
   .set_align(lvgl::Align::Center) // Use Align::Center enum
   .add_flag(lvgl::ObjFlag::Checkable); // Use ObjFlag enum
```

### 4. Styles: Fluent Proxy API
The `Stylable` mixin (flat setters like `set_style_bg_color`) has been replaced with a `style()` proxy factory.

**Old Code:**
```cpp
btn.set_style_bg_color(Color::Red, LV_PART_MAIN);
btn.set_style_radius(5, 0);
```

**New Code:**
```cpp
btn.style()
   .bg_color(Color::Red)
   .radius(5);

// Specific state
btn.style(State::Pressed)
   .bg_color(Color::Blue);
```

## Creating custom widgets
Inherit from `lvgl::Widget<MyWidget>` to get all mixins for free.

```cpp
class MyWidget : public lvgl::Widget<MyWidget> {
public:
    MyWidget() : Widget(lv_my_widget_create(lv_screen_active()), Ownership::Managed) {}
    
    // Add custom fluent methods
    MyWidget& set_custom_prop(int val) {
        // ...
        return *this;
    }
};
```
### 4. Scoped Enums (Strict Typing)
Use C++ Scoped Enums instead of raw macros for improved type safety. Many methods that previously accepted both now strictly require the C++ enum.
- `LV_STATE_PRESSED` -> `lvgl::State::Pressed`
- `LV_PART_MAIN` -> `lvgl::Part::Main`
- `LV_ALIGN_CENTER` -> `lvgl::Align::Center`
- `LV_FLEX_FLOW_ROW` -> `lvgl::FlexFlow::Row`
- `LV_FLEX_ALIGN_CENTER` -> `lvgl::FlexAlign::Center`
- `LV_PALETTE_BLUE` -> `lvgl::Palette::Blue`

Bitwise operations are supported for flag-like enums:
```cpp
btn.style().set_pad_all(10).add_state(State::Checked | State::Focused);
```

### 5. Advanced Callbacks
You can now use `std::function`, lambdas, and method binders directly.
```cpp
btn.on_clicked([](lvgl::Event& e) {
    std::cout << "Clicked!" << std::endl;
});
```

### 6. New Manager Classes and Proxies
- **Screen**: `lvgl::Screen` for dedicated screen objects.
- **Group**: `lvgl::Group` for input device groups.
- **FileSystem**: `lvgl::File` and `lvgl::Directory` (stdio wrapper) using `FsRes`, `FsMode`, and `FsWhence` enums.
- **Images**: `lvgl::Image` with `ImageDescriptor` resource management.
- **Proxies**: `style()`, `scroll()`, `layout()`, `tree()`, and `interaction()` offer specialized API sections without bloat.

### 7. Core Object Lifecycle
Core objects like `Display` and `InputDevice` now support **Move Semantics**. They are non-copyable to prevent accidental duplication of global handles but can be moved into storage.

```cpp
lvgl::Display disp = lvgl::Display::create(800, 480);
std::vector<lvgl::Display> displays;
displays.push_back(std::move(disp)); // Transfer ownership
```
