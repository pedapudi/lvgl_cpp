#ifndef LVGL_CPP_MISC_COLOR_H_
#define LVGL_CPP_MISC_COLOR_H_

#include <cstdint>

#include "enums.h"
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
  Color(Palette p);
  Color(uint32_t hex);  // 0xRRGGBB

  // Implicit conversion to lv_color_t
  operator lv_color_t() const { return color_; }

  // Static creators
  static Color white();
  static Color black();
  /**
   * @brief Create a color from RGB values.
   * @param r Red component (0-255).
   * @param g Green component (0-255).
   * @param b Blue component (0-255).
   */
  static Color make(uint8_t r, uint8_t g, uint8_t b);

  /**
   * @brief Create a color from a hex integer (0xRRGGBB).
   */
  static Color from_hex(uint32_t hex);

  /**
   * @brief Create a color from HSV values.
   * @param h Hue (0-360).
   * @param s Saturation (0-100).
   * @param v Value (0-100).
   */
  static Color from_hsv(uint16_t h, uint8_t s, uint8_t v);

  // Operations
  uint32_t to_int() const;
  bool operator==(const Color& other) const;
  bool operator!=(const Color& other) const;

  // Arithmetic operators (clamped to 0-255)
  Color operator+(const Color& other) const;
  Color operator-(const Color& other) const;
  Color operator*(float factor) const;
  Color operator/(float factor) const;

  // Bitwise operators (raw component-wise)
  Color operator&(const Color& other) const;
  Color operator|(const Color& other) const;
  Color operator^(const Color& other) const;
  Color operator~() const;

  /**
   * @brief Mix this color with another color.
   * @param other The other color.
   * @param ratio Ratio of the mix (0: fully other, 255: fully this).
   * @return The mixed color.
   */
  Color mix(const Color& other, uint8_t ratio) const;

  /**
   * @brief Lighten the color.
   * @param lvl Amount to lighten.
   * @return Lightened color.
   */
  Color lighten(lv_opa_t lvl) const;

  /**
   * @brief Darken the color.
   * @param lvl Amount to darken.
   * @return Darkened color.
   */
  Color darken(lv_opa_t lvl) const;

  /**
   * @brief Get the luminance of the color.
   * @return Luminance (0-255).
   */
  uint8_t luminance() const;

  /**
   * @brief Get the brightness of the color.
   * @return Brightness (0-255).
   */
  uint8_t brightness() const;

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
