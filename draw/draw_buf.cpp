#include "draw_buf.h"

#include <utility>

namespace lvgl {
namespace draw {

DrawBuf::DrawBuf(uint32_t w, uint32_t h, lv_color_format_t cf, uint32_t stride)
    : buf_(lv_draw_buf_create(w, h, cf, stride)), owns_(true) {}

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

lv_color_format_t DrawBuf::format() const {
  return buf_ ? static_cast<lv_color_format_t>(buf_->header.cf)
              : LV_COLOR_FORMAT_UNKNOWN;
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

}  // namespace draw
}  // namespace lvgl
