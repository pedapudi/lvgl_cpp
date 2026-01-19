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

ImageDescriptor::ImageDescriptor(uint32_t w, uint32_t h, lv_color_format_t cf,
                                 const uint8_t* data, uint32_t data_size) {
  dsc_.header.w = w;
  dsc_.header.h = h;
  dsc_.header.cf = cf;
  dsc_.header.magic = LV_IMAGE_HEADER_MAGIC;
  dsc_.data_size = data_size;

  // Allocate and copy data
  owned_data_.resize(data_size);
  std::memcpy(owned_data_.data(), data, data_size);
  dsc_.data = owned_data_.data();
  owns_data_ = true;
}

ImageDescriptor::~ImageDescriptor() {
  // owned_data_ cleans itself up
}

ImageDescriptor::ImageDescriptor(const ImageDescriptor& other) {
  dsc_ = other.dsc_;
  owns_data_ = other.owns_data_;
  if (owns_data_) {
    owned_data_ = other.owned_data_;  // Deep copy data
    dsc_.data = owned_data_.data();   // Point to new data
  }
}

ImageDescriptor& ImageDescriptor::operator=(const ImageDescriptor& other) {
  if (this != &other) {
    dsc_ = other.dsc_;
    owns_data_ = other.owns_data_;
    if (owns_data_) {
      owned_data_ = other.owned_data_;
      dsc_.data = owned_data_.data();
    }
  }
  return *this;
}

ImageDescriptor::ImageDescriptor(ImageDescriptor&& other) noexcept {
  dsc_ = other.dsc_;
  owns_data_ = other.owns_data_;
  owned_data_ = std::move(other.owned_data_);
  // Update pointer if we owned data
  if (owns_data_) {
    dsc_.data = owned_data_.data();
  }

  // Reset other
  std::memset(&other.dsc_, 0, sizeof(other.dsc_));
  other.owns_data_ = false;
}

ImageDescriptor& ImageDescriptor::operator=(ImageDescriptor&& other) noexcept {
  if (this != &other) {
    dsc_ = other.dsc_;
    owns_data_ = other.owns_data_;
    owned_data_ = std::move(other.owned_data_);
    if (owns_data_) {
      dsc_.data = owned_data_.data();
    }

    std::memset(&other.dsc_, 0, sizeof(other.dsc_));
    other.owns_data_ = false;
  }
  return *this;
}

const lv_image_dsc_t* ImageDescriptor::raw() const { return &dsc_; }

bool ImageDescriptor::is_valid() const {
  return dsc_.header.magic == LV_IMAGE_HEADER_MAGIC;
}

}  // namespace lvgl
