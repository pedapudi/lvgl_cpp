#ifndef LVGL_CPP_DRAW_DRAW_BUF_H_
#define LVGL_CPP_DRAW_DRAW_BUF_H_

#include <cstdint>

#include "../misc/enums.h"
#include "lvgl.h"

/**
 * @file draw_buf.h
 * @brief User Guide:
 * `DrawBuf` is an RAII wrapper for `lv_draw_buf_t`. It manages the memory
 * required for drawing operations, such as display buffers or off-screen
 * canvases.
 *
 * Key Features:
 * - **RAII**: Automatically allocates and deallocates buffer memory.
 * - **Endianness**: Provides optimized byte-swapping (useful for SPI displays).
 * - **Flags**: Easy management of image flags (e.g. static, pre-multiplied).
 * - **Type Safety**: Prefer `lvgl_cpp` color formats and dimension types.
 */

namespace lvgl {
namespace draw {

/**
 * @brief Drawing buffer management.
 */
class DrawBuf {
 public:
  /**
   * @brief Create a draw buffer with specified dimensions and format.
   * @param w Width in pixels.
   * @param h Height in pixels.
   * @param cf Color format (default ARGB8888).
   * @param stride Stride in bytes (0 for auto).
   */
  DrawBuf(uint32_t w, uint32_t h, ColorFormat cf = ColorFormat::ARGB8888,
          uint32_t stride = 0);

  /**
   * @brief Wrap an existing C draw buffer.
   * @param buf Pointer to the C struct.
   * @param take_ownership If true, will destroy the buffer in destructor.
   */
  explicit DrawBuf(lv_draw_buf_t* buf, bool take_ownership = false);

  ~DrawBuf();

  // Non-copyable
  DrawBuf(const DrawBuf&) = delete;
  DrawBuf& operator=(const DrawBuf&) = delete;

  // Movable
  DrawBuf(DrawBuf&& other) noexcept;
  DrawBuf& operator=(DrawBuf&& other) noexcept;

  /**
   * @brief Get the underlying C struct.
   */
  lv_draw_buf_t* raw() const { return buf_; }

  /** @brief Get width in pixels. */
  uint32_t width() const;

  /** @brief Get height in pixels. */
  uint32_t height() const;

  /** @brief Get color format. */
  ColorFormat format() const;

  /**
   * @brief Clear the buffer (fill with 0/transparent).
   * @param area Specific area to clear, or nullptr for the whole buffer.
   */
  void clear(const lv_area_t* area = nullptr);

  /**
   * @brief Set a flag on the draw buffer.
   */
  void set_flag(lv_image_flags_t flag);

  /**
   * @brief Clear a flag on the draw buffer.
   */
  void clear_flag(lv_image_flags_t flag);

  /**
   * @brief Get pointer to raw data.
   */
  void* data();

  /**
   * @brief Get pointer to raw data (const).
   */
  const void* data() const;

  /**
   * @brief Get total size of the buffer in bytes.
   */
  size_t data_size() const;

  /**
   * @brief Swap the endianness of the buffer in-place.
   * Optimized for 16-bit (RGB565) and 32-bit (ARGB8888) formats.
   */
  void swap_endianness();

  /**
   * @brief Premultiply the buffer color with alpha.
   * Only applicable to formats with an alpha channel.
   */
  void premultiply();

 private:
  lv_draw_buf_t* buf_ = nullptr;
  bool owns_ = false;
};

}  // namespace draw

using draw::DrawBuf;

}  // namespace lvgl

#endif  // LVGL_CPP_DRAW_DRAW_BUF_H_
