# Integrating LVGL Fonts within C++

## Executive Summary

Fonts in LVGL (`lv_font_t`) are treated as resources rather than widgets. They are typically:
1.  **Global Static Objects**: Compiled into the binary (e.g., `lv_font_montserrat_14`).
2.  **Runtime Loaded Assets**: Loaded from a filesystem into memory (e.g., parsing a `.bin` or `.ttf` file).

The current `lvgl_cpp` library has a basic `Font` wrapper that only supports *viewing* existing fonts. To provide a complete, idiomatic C++ experience, we must support **ownership management** (RAII for loaded fonts) and seamless integration with the `Style` system.

## Requirements and Dependencies

| Requirement | Description |
| :--- | :--- |
| **LVGL Version** | v9.0+ |
| **Filesystem** | Required only for runtime-loaded fonts (Bin/TTF). |
| **Font Engine** | `lv_libs` (FreeType/TinyTTF) if using vector fonts. |

## Configuration and Setup

### 1. Enabling Built-in Fonts
Built-in fonts must be enabled in `lv_conf.h` before they can be used.

```c
// lv_conf.h
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_24 1
#define LV_FONT_DEFAULT &lv_font_montserrat_14
```

### 2. Custom Fonts (C-Array)
Use the [LVGL Online Font Converter](https://lvgl.io/tools/fontconverter) to convert `.ttf` to a C file.
*   **Settings**: Choose size, bpp (anti-aliasing), and range (ASCII/Symbols).
*   **Integration**: Add the generated `.c` file to your CMake target.

## Design Proposal: The "Font Identity"

We propose a dual-class design pattern: `Font` (View) and `OwnedFont` (Owner).

### 1. The `Font` View (Existing and Enhanced)
A lightweight wrapper around `const lv_font_t*`. It does *not* own the memory.

```cpp
class Font {
 public:
  // Implicit construction from raw pointer allowed for seamless wrapping
  Font(const lv_font_t* font); // NOLINT

  // Accessors
  int32_t get_line_height() const;
  
  // Static Built-ins (Compile-time check)
  static const Font& montserrat_14();
};
```

### 2. The `OwnedFont` (New)
A resource-owning class for runtime-loaded fonts. It inherits from `Font` (or implicitly converts) but manages lifecycle.

```cpp
class OwnedFont : public Font {
 public:
  /**
   * @brief Load a binary font from filesystem.
   * @param path Path to .bin or .ttf file.
   */
  static OwnedFont load(const std::string& path);
  
  // Rule of 5: Move-only
  OwnedFont(const OwnedFont&) = delete;
  OwnedFont(OwnedFont&& other) noexcept;
  ~OwnedFont(); // Calls lv_font_free() or equivalent
};
```

### 3. Style Integration
The `Style` class needs a direct setter.

```cpp
// core/style.h
class Style {
  // ...
  void set_text_font(const Font& font); // Accepts both Font and OwnedFont
};
```

### 4. Widget Integration
Widgets should accept fonts directly via `set_style_...` wrappers or convenience methods.

```cpp
label.set_style_text_font(Font::montserrat_14());
```

## Implementation Guide

### A. Wrapping Built-in Fonts
We continue using static accessors.

```cpp
// Usage
auto f = Font::montserrat_14();
```

### B. Loading Runtime Fonts
For binary fonts (fastest lookup, pre-rasterized):

```cpp
// Usage
try {
    auto custom_font = OwnedFont::load("S:/fonts/my_font.bin");
    label.set_style_text_font(custom_font);
} catch (const LvglError& e) {
    // Handle load failure
}
```

### C. Using Custom C-Array Fonts

For fonts generated as C files, you must declare the C symbol (using existing macros or `extern`) and then wrap it.

Since the `Font` class is a lightweight "View" (essentially a pointer), you should construct it directly at the point of use. There is no need for complex static wrappers or macros.

```cpp
// 1. Declare the C symbol (standard LVGL practice)
LV_FONT_DECLARE(my_custom_font);

// 2. Use directly by wrapping on-the-fly
label.set_style_text_font(Font(&my_custom_font));
```

This approach is:
*   **Idiomatic**: Uses standard C++ constructors.
*   **Efficient**: `Font` is the size of a pointer; construction is free.
*   **Transparent**: No hidden static variables or macro magic.

### D. FreeType / Vector Fonts
If FreeType is enabled, `lv_freetype_font_create` is used. `OwnedFont` should potentially have a factory for this, or a subclass `FreeTypeFont`.

```cpp
auto ft_font = FreeTypeFont::create("S:/fonts/arial.ttf", 24_px);
```

## Tooling and Workflow

### Font Converter
The primary tool for embedded systems is the `lv_font_conv` Node.js tool (or the online version).

```bash
# Install
npm install -g lv_font_conv

# Convert
lv_font_conv --no-compress --format lvgl --font Roboto-Regular.ttf \
  --range 0x20-0x7F --bpp 4 --size 24 --output my_font_24.c
```

## References and Documentation
*   [Official LVGL Font Documentation](https://docs.lvgl.io/9.4/details/main-modules/font.html)
*   [Online Font Converter](https://lvgl.io/tools/fontconverter)
*   [LVGL Font API](https://docs.lvgl.io/master/API/font/index.html)

