#ifndef LVGL_CPP_DRAW_DRAW_BUF_H_
#define LVGL_CPP_DRAW_DRAW_BUF_H_

#include <cstdint>

#include "lvgl.h"

namespace lvgl {
namespace draw {

/**
 * @brief RAII wrapper for lv_draw_buf_t.
 * Manages allocation and deallocation of an LVGL drawing buffer.
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
  DrawBuf(uint32_t w, uint32_t h,
          lv_color_format_t cf = LV_COLOR_FORMAT_ARGB8888, uint32_t stride = 0);

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
  lv_color_format_t format() const;

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

 private:
  lv_draw_buf_t* buf_ = nullptr;
  bool owns_ = false;
};

}  // namespace draw

using draw::DrawBuf;

}  // namespace lvgl

#endif  // LVGL_CPP_DRAW_DRAW_BUF_H_
