#include "owned_font.h"

#include <utility>

namespace lvgl {

OwnedFont::OwnedFont() : Font(nullptr) {}

OwnedFont::OwnedFont(lv_font_t* font) : Font(font) {}

OwnedFont::OwnedFont(OwnedFont&& other) noexcept : Font(other.font_) {
  // Take ownership, clear source
  other.font_ = nullptr;
}

OwnedFont& OwnedFont::operator=(OwnedFont&& other) noexcept {
  if (this != &other) {
    destroy();
    font_ = other.font_;
    other.font_ = nullptr;
  }
  return *this;
}

OwnedFont::~OwnedFont() { destroy(); }

void OwnedFont::destroy() {
  if (font_) {
    // Cast away constness because lv_binfont_destroy takes non-const pointer
    // This is safe because OwnedFont is created with ownership
    lv_binfont_destroy(const_cast<lv_font_t*>(font_));
    font_ = nullptr;
  }
}

OwnedFont OwnedFont::load_bin(const std::string& path) {
  // lv_binfont_create returns a new font object or NULL on failure
  lv_font_t* f = lv_binfont_create(path.c_str());
  return OwnedFont(f);
}

}  // namespace lvgl
