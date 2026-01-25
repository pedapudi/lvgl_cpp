# Style System Design

## 1. Overview
The Style System replaces raw `lv_style_t` manipulation with a strongly-typed, fluent C++ builder. This allows styles to be defined declaratively and safely.

## 2. The `Style` Builder
The `lvgl::Style` class wraps `lv_style_t` and provides method chaining for all standard LVGL properties.

### Features
*   **Fluent Interface**: `style.bg_color(Red).radius(10)`
*   **Type Safety**: Uses `Color`, `Opacity`, `Align` classes/enums instead of raw integers.
*   **RAII**: Automatically initializes and resets the style.

### API Reference

```cpp
class Style {
 public:
  Style(); // Initializes lv_style_init
  
  // === Background ===
  Style& bg_color(Color color);
  Style& bg_opa(Opacity opa);
  Style& bg_grad_color(Color start, Color end);
  Style& bg_grad_dir(GradDir dir);
  Style& bg_main_stop(int32_t value);
  Style& bg_grad_stop(int32_t value);
  Style& bg_image(const ImageDescriptor& img);
  
  // === Border ===
  Style& border_color(Color color);
  Style& border_width(int32_t width);
  Style& border_opa(Opacity opa);
  Style& border_side(BorderSide side);
  
  // === Outline & Shadow ===
  Style& outline_width(int32_t width);
  Style& outline_color(Color color);
  Style& outline_pad(int32_t pad);
  Style& shadow_width(int32_t width);
  Style& shadow_ofs(int32_t x, int32_t y);
  
  // === Padding & Sizing ===
  Style& pad_all(int32_t pad);
  Style& pad_hor(int32_t pad);
  Style& pad_ver(int32_t pad);
  Style& pad_gap(int32_t gap);
  Style& width(int32_t w);
  Style& height(int32_t h);
  Style& min_width(int32_t w);
  Style& max_width(int32_t w);
  
  // === Text ===
  Style& text_color(Color color);
  Style& text_font(const Font& font);
  Style& text_align(TextAlign align);
  Style& text_letter_space(int32_t px);
  Style& text_line_space(int32_t px);
  
  // === Image & Line ===
  Style& image_opa(Opacity opa);
  Style& image_recolor(Color color);
  Style& line_width(int32_t width);
  Style& line_color(Color color);

  // === Animation ===
  Style& anim_time(uint32_t ms);
  Style& transition(const lv_style_transition_dsc_t* props);
  
  // Accessor
  lv_style_t* raw();
};
```

## 3. Usage Examples

### Basic Button Style
```cpp
Style btn_style;
btn_style.bg_color(Palette::Blue)
         .bg_grad_color(Palette::Blue, Palette::Cyan)
         .bg_grad_dir(GradDir::Ver)
         .radius(5)
         .border_width(2)
         .border_color(Palette::Silver)
         .text_color(Color::White);

button.add_style(btn_style, Part::Main);
```

### Local Styles
Objects support direct local style modification without creating a full `Style` object via the `Stylable` mixin.

```cpp
// Direct Proxy
button.set_bg_color(Palette::Red)
      .set_border_width(0);
```

