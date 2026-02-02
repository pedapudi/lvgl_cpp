#include "draw_buf.h"

#include <utility>

namespace lvgl {
namespace draw {

DrawBuf::DrawBuf(uint32_t w, uint32_t h, ColorFormat cf, uint32_t stride)
    : buf_(
          lv_draw_buf_create(w, h, static_cast<lv_color_format_t>(cf), stride)),
      owns_(true) {}

DrawBuf::DrawBuf(lv_draw_buf_t* buf, bool take_ownership)
    : buf_(buf), owns_(take_ownership) {}

DrawBuf::~DrawBuf() {
  if (buf_ && owns_) {
    lv_draw_buf_destroy(buf_);
  }
}

DrawBuf::DrawBuf(DrawBuf&& other) noexcept
    : buf_(other.buf_), owns_(other.owns_) {
  other.buf_ = nullptr;
  other.owns_ = false;
}

DrawBuf& DrawBuf::operator=(DrawBuf&& other) noexcept {
  if (this != &other) {
    if (buf_ && owns_) {
      lv_draw_buf_destroy(buf_);
    }
    buf_ = other.buf_;
    owns_ = other.owns_;
    other.buf_ = nullptr;
    other.owns_ = false;
  }
  return *this;
}

uint32_t DrawBuf::width() const { return buf_ ? buf_->header.w : 0; }

uint32_t DrawBuf::height() const { return buf_ ? buf_->header.h : 0; }

ColorFormat DrawBuf::format() const {
  return buf_ ? static_cast<ColorFormat>(buf_->header.cf)
              : ColorFormat::Unknown;
}

void DrawBuf::clear(const lv_area_t* area) {
  if (buf_) {
    lv_draw_buf_clear(buf_, area);
  }
}

void DrawBuf::set_flag(lv_image_flags_t flag) {
  if (buf_) {
    lv_draw_buf_set_flag(buf_, flag);
  }
}

void DrawBuf::clear_flag(lv_image_flags_t flag) {
  if (buf_) {
    lv_draw_buf_clear_flag(buf_, flag);
  }
}

void* DrawBuf::data() { return buf_ ? buf_->data : nullptr; }

const void* DrawBuf::data() const { return buf_ ? buf_->data : nullptr; }

size_t DrawBuf::data_size() const { return buf_ ? buf_->data_size : 0; }

void DrawBuf::swap_endianness() {
  if (!buf_ || !buf_->data) return;

  ColorFormat cf = format();
  if (cf == ColorFormat::RGB565) {
    lv_draw_sw_rgb565_swap(buf_->data, width() * height());
  } else if (cf == ColorFormat::ARGB8888 || cf == ColorFormat::XRGB8888) {
    uint32_t* p = reinterpret_cast<uint32_t*>(buf_->data);
    size_t count = (width() * height());
    for (size_t i = 0; i < count; ++i) {
      p[i] = __builtin_bswap32(p[i]);
    }
  } else {
    // Generic fallback for 16-bit if not exactly RGB565 but still 2 bytes
    if (LV_COLOR_FORMAT_GET_BPP(static_cast<lv_color_format_t>(cf)) == 16) {
      uint16_t* p = reinterpret_cast<uint16_t*>(buf_->data);
      size_t count = (width() * height());
      for (size_t i = 0; i < count; ++i) {
        p[i] = __builtin_bswap16(p[i]);
      }
    }
  }
}

void DrawBuf::premultiply() {
  if (buf_) {
    lv_draw_buf_premultiply(buf_);
  }
}

}  // namespace draw
}  // namespace lvgl
