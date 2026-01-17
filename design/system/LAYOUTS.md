# Style System Design

## 1. Overview
The Layouts & System design focuses on replacing raw enums and untyped arrays with strongly typed, readable builders.

## 2. Style Builder
A fluent builder for `lv_style_t`.

```cpp
class Style {
public:
    Style(); // inits lv_style_t
    
    Style& bg_color(Color c);
    Style& bg_opa(Opacity o);
    Style& border_width(int32_t w);
    Style& border_color(Color c);
    Style& radius(int32_t r);
    
    // ... all 100+ properties
    
    const lv_style_t* raw() const;
};
```

## 3. Layouts
LVGL uses `lv_flex_flow_t` and `lv_grid_dsc_t` arrays.

### 3.1. Flex Layout
```cpp
struct Flex {
    static const Flex Row() { return Flex(LV_FLEX_FLOW_ROW); }
    static const Flex Column() { return Flex(LV_FLEX_FLOW_COLUMN); }
    
    Flex& gap(int32_t row_gap, int32_t col_gap);
    Flex& align(lv_flex_align_t main, lv_flex_align_t cross, lv_flex_align_t track);
};

// Usage
obj.layout(Flex::Row().gap(10).center());
```

### 3.2. Grid Layout
Grids require static arrays for track descriptions. This is tricky in C++.

**Proposal**: A helper that manages `static` storage for simple grids or wraps `std::vector` for dynamic ones (aware of lifetime issues).

```cpp
// Static Grid Helper
struct Grid {
    template <size_t C, size_t R>
    static void apply(Object& obj, const int32_t (&cols)[C], const int32_t (&rows)[R]);
};
```

## 4. Drivers (Display & InDev)
Wrappers for `lv_display_t` and `lv_indev_t`.

### 4.1. Display
*   `Display::create(w, h)`
*   `Display::set_buffers(...)`
*   `Display::set_flush_cb(...)`

This is critical for the ESP32 setup code.
