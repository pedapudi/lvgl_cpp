#ifndef LVGL_CPP_MISC_COLOR_H_
#define LVGL_CPP_MISC_COLOR_H_

#include <cstdint>

#include "lvgl.h"  // IWYU pragma: export

namespace lvgl {

/**
 * @brief C++ wrapper for lv_color_t and related operations.
 * This class is lightweight and can be treated as a value type.
 */
class Color {
 public:
  Color();
  Color(lv_color_t c);
  Color(uint8_t r, uint8_t g, uint8_t b);
  Color(uint32_t hex);  // 0xRRGGBB

  // Implicit conversion to lv_color_t
  operator lv_color_t() const { return color_; }

  // Static creators
  static Color white();
  static Color black();
  static Color make(uint8_t r, uint8_t g, uint8_t b);
  static Color from_hex(uint32_t hex);
  static Color from_hsv(uint16_t h, uint8_t s, uint8_t v);

  // Operations
  uint32_t to_int() const;
  bool operator==(const Color& other) const;
  bool operator!=(const Color& other) const;

  Color mix(const Color& other, uint8_t ratio) const;  // 0: other, 255: this
  Color lighten(lv_opa_t lvl) const;
  Color darken(lv_opa_t lvl) const;

  uint8_t luminance() const;

  // Accessors (using struct access)
  uint8_t r() const { return color_.red; }
  uint8_t g() const { return color_.green; }
  uint8_t b() const { return color_.blue; }

  void set_r(uint8_t r) { color_.red = r; }
  void set_g(uint8_t g) { color_.green = g; }
  void set_b(uint8_t b) { color_.blue = b; }

 private:
  lv_color_t color_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_COLOR_H_
