#include "font.h"

namespace lvgl {

Font::Font(const lv_font_t* font) : font_(font) {}

int32_t Font::get_line_height() const {
  return font_ ? lv_font_get_line_height(font_) : 0;
}

int32_t Font::get_base_line() const { return font_ ? font_->base_line : 0; }

uint16_t Font::get_glyph_width(uint32_t letter, uint32_t letter_next) const {
  return font_ ? lv_font_get_glyph_width(font_, letter, letter_next) : 0;
}

// Static Font definitions
// We use a small optimization to avoid creating wrapper objects repeatedly
// by returning references to static wrappers.
// However, since Font is just a pointer, copying is cheap.
// We'll return references to static instances.

#define DEFINE_FONT_ACCESSOR(name, lv_name) \
  const Font& Font::name() {                \
    static Font f(&lv_name);                \
    return f;                               \
  }

#if LV_FONT_MONTSERRAT_8
DEFINE_FONT_ACCESSOR(montserrat_8, lv_font_montserrat_8)
#else
const Font& Font::montserrat_8() {
  static Font f(nullptr);
  return f;
}
#endif

#if LV_FONT_MONTSERRAT_10
DEFINE_FONT_ACCESSOR(montserrat_10, lv_font_montserrat_10)
#else
const Font& Font::montserrat_10() {
  static Font f(nullptr);
  return f;
}
#endif

#if LV_FONT_MONTSERRAT_12
DEFINE_FONT_ACCESSOR(montserrat_12, lv_font_montserrat_12)
#else
const Font& Font::montserrat_12() {
  static Font f(nullptr);
  return f;
}
#endif

#if LV_FONT_MONTSERRAT_14
DEFINE_FONT_ACCESSOR(montserrat_14, lv_font_montserrat_14)
#else
const Font& Font::montserrat_14() {
  static Font f(nullptr);
  return f;
}
#endif

#if LV_FONT_MONTSERRAT_16
DEFINE_FONT_ACCESSOR(montserrat_16, lv_font_montserrat_16)
#else
const Font& Font::montserrat_16() {
  static Font f(nullptr);
  return f;
}
#endif

#if LV_FONT_MONTSERRAT_18
DEFINE_FONT_ACCESSOR(montserrat_18, lv_font_montserrat_18)
#else
const Font& Font::montserrat_18() {
  static Font f(nullptr);
  return f;
}
#endif

#if LV_FONT_MONTSERRAT_20
DEFINE_FONT_ACCESSOR(montserrat_20, lv_font_montserrat_20)
#else
const Font& Font::montserrat_20() {
  static Font f(nullptr);
  return f;
}
#endif

#if LV_FONT_MONTSERRAT_22
DEFINE_FONT_ACCESSOR(montserrat_22, lv_font_montserrat_22)
#else
const Font& Font::montserrat_22() {
  static Font f(nullptr);
  return f;
}
#endif

#if LV_FONT_MONTSERRAT_24
DEFINE_FONT_ACCESSOR(montserrat_24, lv_font_montserrat_24)
#else
const Font& Font::montserrat_24() {
  static Font f(nullptr);
  return f;
}
#endif

#if LV_FONT_MONTSERRAT_26
DEFINE_FONT_ACCESSOR(montserrat_26, lv_font_montserrat_26)
#else
const Font& Font::montserrat_26() {
  static Font f(nullptr);
  return f;
}
#endif

#if LV_FONT_MONTSERRAT_28
DEFINE_FONT_ACCESSOR(montserrat_28, lv_font_montserrat_28)
#else
const Font& Font::montserrat_28() {
  static Font f(nullptr);
  return f;
}
#endif

#if LV_FONT_MONTSERRAT_30
DEFINE_FONT_ACCESSOR(montserrat_30, lv_font_montserrat_30)
#else
const Font& Font::montserrat_30() {
  static Font f(nullptr);
  return f;
}
#endif

#if LV_FONT_MONTSERRAT_32
DEFINE_FONT_ACCESSOR(montserrat_32, lv_font_montserrat_32)
#else
const Font& Font::montserrat_32() {
  static Font f(nullptr);
  return f;
}
#endif

const Font& Font::get_default() {
  static Font f(lv_font_default());
  return f;
}

}  // namespace lvgl
