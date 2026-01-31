#include "owned_font.h"

#include <utility>

namespace lvgl {

OwnedFont::OwnedFont() : Font(nullptr) {}

OwnedFont::OwnedFont(lv_font_t* font, FontType type)
    : Font(font), type_(type) {}

OwnedFont::OwnedFont(OwnedFont&& other) noexcept
    : Font(other.font_), type_(other.type_) {
  // Take ownership, clear source
  other.font_ = nullptr;
  other.type_ = FontType::None;
}

OwnedFont& OwnedFont::operator=(OwnedFont&& other) noexcept {
  if (this != &other) {
    destroy();
    font_ = other.font_;
    type_ = other.type_;
    other.font_ = nullptr;
    other.type_ = FontType::None;
  }
  return *this;
}

OwnedFont::~OwnedFont() { destroy(); }

void OwnedFont::destroy() {
  if (font_) {
    lv_font_t* f = const_cast<lv_font_t*>(font_);
    if (type_ == FontType::TinyTTF) {
#if LV_USE_TINY_TTF
      lv_tiny_ttf_destroy(f);
#endif
    } else if (type_ == FontType::Binary) {
      lv_binfont_destroy(f);
    }
    font_ = nullptr;
    type_ = FontType::None;
  }
}

OwnedFont OwnedFont::load_bin(const std::string& path) {
  // lv_binfont_create returns a new font object or NULL on failure
  lv_font_t* f = lv_binfont_create(path.c_str());
  return OwnedFont(f, FontType::Binary);
}

#if LV_USE_TINY_TTF
OwnedFont OwnedFont::load_tiny_ttf(const void* data, size_t data_size,
                                   int32_t font_size) {
  lv_font_t* f = lv_tiny_ttf_create_data(data, data_size, font_size);
  return OwnedFont(f, FontType::TinyTTF);
}
#endif

}  // namespace lvgl
