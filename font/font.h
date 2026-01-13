#ifndef LVGL_CPP_FONT_FONT_H_
#define LVGL_CPP_FONT_FONT_H_

#include "lvgl.h" // IWYU pragma: export

namespace lvgl {

/**
 * @brief Wrapper for lv_font_t.
 * Typically fonts are global constants, so this wrapper holds a pointer.
 */
class Font {
public:
  explicit Font(const lv_font_t *font);

  // Accessors
  const lv_font_t *raw() const { return font_; }
  bool is_valid() const { return font_ != nullptr; }

  // Properties
  int32_t get_line_height() const;
  int32_t get_base_line() const;

  // Glyph operations
  uint16_t get_glyph_width(uint32_t letter, uint32_t letter_next = 0) const;

  // Static access to built-in fonts if enabled in lv_conf.h
  // Returns nullptr if not enabled.
  static const Font &montserrat_8();
  static const Font &montserrat_10();
  static const Font &montserrat_12();
  static const Font &montserrat_14();
  static const Font &montserrat_16();
  static const Font &montserrat_18();
  static const Font &montserrat_20();
  static const Font &montserrat_22();
  static const Font &montserrat_24();
  static const Font &montserrat_26();
  static const Font &montserrat_28();
  static const Font &montserrat_30();
  static const Font &montserrat_32();

private:
  const lv_font_t *font_;
};

} // namespace lvgl

#endif // LVGL_CPP_FONT_FONT_H_
