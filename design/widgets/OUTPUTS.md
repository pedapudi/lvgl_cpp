# Output & Visual Widgets Design

## 1. Scope
This document covers widgets used primarily for displaying information:
*   `Label` (`lv_label`)
*   `Bar` (`lv_bar`)
*   `Arc` (`lv_arc`)
*   `LED` (`lv_led`)
*   `Spinner` (`lv_spinner`)
*   `Image` (`lv_image`)
*   `Line` (`lv_line`)

## 2. Widget Specifications

### 2.1. Label (`Label`)
*   **Base**: `Widget<Label>`
*   **Methods**:
    *   `Label& text(const std::string&)`
    *   `Label& text_fmt(const char* fmt, ...)`
    *   `Label& long_mode(lv_label_long_mode_t)`
    *   `Label& recolor(bool)`
*   **Helper**:
    We should enable `Button` and other inputs to auto-create labels, but `Label` itself needs to be robust.

### 2.2. Bar (`Bar`)
*   **Base**: `Widget<Bar>`
*   **Methods**:
    *   `Bar& range(int32_t min, int32_t max)`
    *   `Bar& value(int32_t val, bool anim = false)`
    *   `Bar& start_value(int32_t val)`

### 2.3. Arc (`Arc`)
*   **Base**: `Widget<Arc>`
*   **Methods**:
    *   `Arc& angles(uint16_t start, uint16_t end)`
    *   `Arc& bg_angles(uint16_t start, uint16_t end)`
    *   `Arc& rotation(uint16_t)`
    *   `Arc& value(int32_t)`

### 2.4. LED (`Led`)
*   **Base**: `Widget<Led>`
*   **Methods**:
    *   `Led& color(Color c)`
    *   `Led& brightness(uint8_t)`
    *   `Led& on()`
    *   `Led& off()`
    *   `Led& toggle()`

### 2.5. Image (`Image`)
*   **Base**: `Widget<Image>`
*   **Methods**:
    *   `Image& src(const void* src)` (Pointer to descriptor or path)
    *   `Image& offset(int32_t x, int32_t y)`
    *   `Image& pivot(int32_t x, int32_t y)`
    *   `Image& angle(int16_t)`
    *   `Image& zoom(uint16_t)`

## 3. Style Integration
Visual widgets heavily rely on Styles for look-and-feel. The new API must ensure `style()` helper works seamlessly.

```cpp
Led(parent).on().style().bg_color(Palette::Red).shadow_width(10);
```
