#include "image_descriptor.h"

#include <cstring>

#include "lvgl.h"

namespace lvgl {

ImageDescriptor::ImageDescriptor() { std::memset(&dsc_, 0, sizeof(dsc_)); }

ImageDescriptor::ImageDescriptor(const lv_image_dsc_t* dsc) {
  if (dsc) {
    dsc_ = *dsc;  // Copy the descriptor struct, but data pointer refers to
                  // original
    owns_data_ = false;
  } else {
    std::memset(&dsc_, 0, sizeof(dsc_));
  }
}

ImageDescriptor ImageDescriptor::from_svg(const char* svg_src) {
  if (!svg_src) return ImageDescriptor();
  uint32_t len = std::strlen(svg_src) + 1;
  // Image headers for SVG in LVGL use 0 for w/h as they are determined at draw
  // time or from the raw data.
  ImageDescriptor dsc(0, 0, LV_COLOR_FORMAT_RAW,
                      reinterpret_cast<const uint8_t*>(svg_src), len);
  return dsc;
}

ImageDescriptor ImageDescriptor::from_svg(const std::string& svg_src) {
  return from_svg(svg_src.c_str());
}

ImageDescriptor::ImageDescriptor(uint32_t w, uint32_t h, lv_color_format_t cf,
                                 const uint8_t* data, uint32_t data_size) {
  dsc_.header.w = w;
  dsc_.header.h = h;
  dsc_.header.cf = cf;
  dsc_.header.magic = LV_IMAGE_HEADER_MAGIC;
  dsc_.data_size = data_size;

  // Allocate and copy data with alignment
  // LVGL often requires 64-byte alignment for draw buffers
  void* ptr = nullptr;
  if (posix_memalign(&ptr, 64, data_size) == 0) {
    owned_data_ = static_cast<uint8_t*>(ptr);
    std::memcpy(owned_data_, data, data_size);
    dsc_.data = owned_data_;
    owns_data_ = true;
  } else {
    // Allocation failed
    owns_data_ = false;
    owned_data_ = nullptr;
    dsc_.data = nullptr;
  }
}

ImageDescriptor::~ImageDescriptor() {
  if (owns_data_ && owned_data_) {
    std::free(owned_data_);
  }
}

ImageDescriptor::ImageDescriptor(const ImageDescriptor& other) {
  dsc_ = other.dsc_;
  owns_data_ = other.owns_data_;
  if (owns_data_ && other.owned_data_) {
    // Deep copy data with alignment
    void* ptr = nullptr;
    if (posix_memalign(&ptr, 64, dsc_.data_size) == 0) {
      owned_data_ = static_cast<uint8_t*>(ptr);
      std::memcpy(owned_data_, other.owned_data_, dsc_.data_size);
      dsc_.data = owned_data_;
    } else {
      // Allocation failed fallback
      owns_data_ = false;
      owned_data_ = nullptr;
      dsc_.data = nullptr;
    }
  } else {
    owned_data_ = nullptr;
  }
}

ImageDescriptor& ImageDescriptor::operator=(const ImageDescriptor& other) {
  if (this != &other) {
    // Clean up current if needed
    if (owns_data_ && owned_data_) {
      std::free(owned_data_);
    }

    dsc_ = other.dsc_;
    owns_data_ = other.owns_data_;
    if (owns_data_ && other.owned_data_) {
      void* ptr = nullptr;
      if (posix_memalign(&ptr, 64, dsc_.data_size) == 0) {
        owned_data_ = static_cast<uint8_t*>(ptr);
        std::memcpy(owned_data_, other.owned_data_, dsc_.data_size);
        dsc_.data = owned_data_;
      } else {
        owns_data_ = false;
        owned_data_ = nullptr;
        dsc_.data = nullptr;
      }
    } else {
      owned_data_ = nullptr;
    }
  }
  return *this;
}

ImageDescriptor::ImageDescriptor(ImageDescriptor&& other) noexcept {
  dsc_ = other.dsc_;
  owns_data_ = other.owns_data_;
  owned_data_ = other.owned_data_;

  // Reset other
  std::memset(&other.dsc_, 0, sizeof(other.dsc_));
  other.owns_data_ = false;
  other.owned_data_ = nullptr;
}

ImageDescriptor& ImageDescriptor::operator=(ImageDescriptor&& other) noexcept {
  if (this != &other) {
    // Clean up current
    if (owns_data_ && owned_data_) {
      std::free(owned_data_);
    }

    dsc_ = other.dsc_;
    owns_data_ = other.owns_data_;
    owned_data_ = other.owned_data_;

    std::memset(&other.dsc_, 0, sizeof(other.dsc_));
    other.owns_data_ = false;
    other.owned_data_ = nullptr;
  }
  return *this;
}

const lv_image_dsc_t* ImageDescriptor::raw() const { return &dsc_; }

bool ImageDescriptor::is_valid() const {
  return dsc_.header.magic == LV_IMAGE_HEADER_MAGIC;
}

}  // namespace lvgl
