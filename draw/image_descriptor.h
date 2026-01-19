#ifndef LVGL_CPP_DRAW_IMAGE_DESCRIPTOR_H_
#define LVGL_CPP_DRAW_IMAGE_DESCRIPTOR_H_

#include <string>
#include <vector>

#include "lvgl.h"

namespace lvgl {

/**
 * @brief Wrapper for lv_image_dsc_t.
 * Validates and manages image descriptors.
 */
class ImageDescriptor {
 public:
  /**
   * @brief Construct an empty descriptor.
   */
  ImageDescriptor();

  /**
   * @brief Wrap an existing C descriptor.
   * @param dsc Pointer to the descriptor.
   * @note The pointer must remain valid.
   */
  ImageDescriptor(const lv_image_dsc_t* dsc);

  /**
   * @brief Create a valid image descriptor from raw data.
   * @param header Image header (w, h, cf).
   * @param data Raw image data.
   * @param data_size Size of data in bytes.
   */
  ImageDescriptor(uint32_t w, uint32_t h, lv_color_format_t cf,
                  const uint8_t* data, uint32_t data_size);

  ~ImageDescriptor();

  // Move-only primarily to manage owned data if we implement that.
  // For now, let's allow copying if it's just a view, but we need to be careful
  // about ownership. If we allocate data, we need proper copy/move. Let's
  // implement full copy/move semantics assuming we might own data.

  ImageDescriptor(const ImageDescriptor& other);
  ImageDescriptor& operator=(const ImageDescriptor& other);
  ImageDescriptor(ImageDescriptor&& other) noexcept;
  ImageDescriptor& operator=(ImageDescriptor&& other) noexcept;

  /**
   * @brief Get the raw descriptor.
   */
  const lv_image_dsc_t* raw() const;

  /**
   * @brief Check if valid.
   */
  bool is_valid() const;

 private:
  lv_image_dsc_t dsc_;
  std::vector<uint8_t> owned_data_;  // If we own the data
  bool owns_data_ = false;
};

}  // namespace lvgl

#endif  // LVGL_CPP_DRAW_IMAGE_DESCRIPTOR_H_
