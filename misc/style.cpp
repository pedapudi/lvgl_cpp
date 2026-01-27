#include "style.h"

#include <utility>

namespace lvgl {

Style::Style() : initialized_(true) { lv_style_init(&style_); }

Style::~Style() {
  if (initialized_) {
    lv_style_reset(&style_);
  }
}

Style::Style(Style&& other) noexcept
    : style_(other.style_), initialized_(other.initialized_) {
  other.initialized_ = false;
}

Style& Style::operator=(Style&& other) noexcept {
  if (this != &other) {
    if (initialized_) {
      lv_style_reset(&style_);
    }
    style_ = other.style_;
    initialized_ = other.initialized_;
    other.initialized_ = false;
  }
  return *this;
}

// =========================================================================
// Raw access
// =========================================================================

lv_style_t* Style::raw() { return &style_; }

const lv_style_t* Style::raw() const { return &style_; }

}  // namespace lvgl
