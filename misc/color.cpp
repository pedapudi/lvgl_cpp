#include "color.h"

#include <algorithm>

namespace lvgl {

Color::Color() : color_{0, 0, 0} {}  // Default black
Color::Color(lv_color_t c) : color_(c) {}
Color::Color(uint8_t r, uint8_t g, uint8_t b)
    : color_(lv_color_make(r, g, b)) {}
Color::Color(Palette p)
    : color_(lv_palette_main(static_cast<lv_palette_t>(p))) {}
Color::Color(uint32_t hex) : color_(lv_color_hex(hex)) {}

Color Color::white() { return Color(lv_color_white()); }
Color Color::black() { return Color(lv_color_black()); }
Color Color::make(uint8_t r, uint8_t g, uint8_t b) {
  return Color(lv_color_make(r, g, b));
}
Color Color::from_hex(uint32_t hex) { return Color(lv_color_hex(hex)); }
Color Color::from_hsv(uint16_t h, uint8_t s, uint8_t v) {
  return Color(lv_color_hsv_to_rgb(h, s, v));
}

uint32_t Color::to_int() const { return lv_color_to_int(color_); }

bool Color::operator==(const Color& other) const {
  return lv_color_eq(color_, other.color_);
}
bool Color::operator!=(const Color& other) const { return !(*this == other); }

Color Color::mix(const Color& other, uint8_t ratio) const {
  // Note: lv_color_mix usage might defer depending on version, checking define.
  // In strict C API, checking implementation.
  // lv_color_mix(c1, c2, ratio) -> mix c1 (fg) and c2 (bg).
  // wrapper: this->mix(other, ratio) -> this is FG, other is BG?
  // Let's assume standard intuitive 'mix this with other'.
  // Actually lv_color_16_16_mix exists, but generalized macro/function is
  // preferable. We'll use manual mixing if needed or lv_color_mix if available.
  // Checking header... lv_color_mix is not explicitly in the header we viewed,
  // but likely a inline/macro. Let's rely on standard lv_color_16_16_mix or
  // similar if defined, else manual. Actually, checking the header again,
  // `lv_color_16_16_mix` is there. We will implement a generic mixing here
  // using standard algorithms if header is missing macro. Wait, `lv_color_mix`
  // is usually a macro in lv_color.h. It seems missing from the viewed header?
  // Ah, might be in lv_color_op.h included. Assuming lv_color_mix works or we
  // use a basic implementation.

  // Fallback implementation for safety and correctness with exposed API
  // ratio: 255 = this, 0 = other.
  uint16_t r = (uint16_t)((uint16_t)color_.red * ratio +
                          (uint16_t)other.color_.red * (255 - ratio)) >>
               8;
  uint16_t g = (uint16_t)((uint16_t)color_.green * ratio +
                          (uint16_t)other.color_.green * (255 - ratio)) >>
               8;
  uint16_t b = (uint16_t)((uint16_t)color_.blue * ratio +
                          (uint16_t)other.color_.blue * (255 - ratio)) >>
               8;
  return Color((uint8_t)r, (uint8_t)g, (uint8_t)b);
}

Color Color::lighten(lv_opa_t lvl) const {
  return Color(lv_color_lighten(color_, lvl));
}
Color Color::darken(lv_opa_t lvl) const {
  return Color(lv_color_darken(color_, lvl));
}

uint8_t Color::luminance() const { return lv_color_luminance(color_); }

uint8_t Color::brightness() const { return lv_color_brightness(color_); }

Color Color::operator+(const Color& other) const {
  return Color((uint8_t)std::min(255, (int)r() + other.r()),
               (uint8_t)std::min(255, (int)g() + other.g()),
               (uint8_t)std::min(255, (int)b() + other.b()));
}

Color Color::operator-(const Color& other) const {
  return Color((uint8_t)std::max(0, (int)r() - other.r()),
               (uint8_t)std::max(0, (int)g() - other.g()),
               (uint8_t)std::max(0, (int)b() - other.b()));
}

Color Color::operator*(float factor) const {
  return Color((uint8_t)std::min(255.0f, std::max(0.0f, (float)r() * factor)),
               (uint8_t)std::min(255.0f, std::max(0.0f, (float)g() * factor)),
               (uint8_t)std::min(255.0f, std::max(0.0f, (float)b() * factor)));
}

Color Color::operator/(float factor) const {
  if (factor == 0.0f) return *this;
  return *this * (1.0f / factor);
}

Color Color::operator&(const Color& other) const {
  return Color(r() & other.r(), g() & other.g(), b() & other.b());
}

Color Color::operator|(const Color& other) const {
  return Color(r() | other.r(), g() | other.g(), b() | other.b());
}

Color Color::operator^(const Color& other) const {
  return Color(r() ^ other.r(), g() ^ other.g(), b() ^ other.b());
}

Color Color::operator~() const { return Color(~r(), ~g(), ~b()); }

}  // namespace lvgl
