#ifndef LVGL_CPP_WIDGETS_LOTTIE_H_
#define LVGL_CPP_WIDGETS_LOTTIE_H_

#include "../core/widget.h"

// Guard for Lottie usage
#if LV_USE_LOTTIE

#include "../../lvgl/src/widgets/lottie/lv_lottie.h"
#include "../misc/animation.h"

namespace lvgl {

/**
 * @brief Lottie animation widget.
 * Wraps lv_lottie object.
 */
class Lottie : public Widget<Lottie> {
 public:
  using Widget::Widget;

  /**
   * @brief Set the source for the animation as an array.
   * @param src Pointer to the data (Lottie JSON converted to array).
   * @param src_size Size of the array in bytes.
   */
  void set_src(const void* src, size_t src_size);

  /**
   * @brief Set the source for the animation as a file path.
   * @param path Path to the JSON file.
   */
  void set_src(const char* path);

  /**
   * @brief Set a buffer for the animation.
   * Defines the size of the animation and provides a rendering buffer.
   * @param w Width.
   * @param h Height.
   * @param buf Pointer to static buffer (w * h * 4 bytes).
   */
  void set_buffer(int32_t w, int32_t h, void* buf);

  /**
   * @brief Set a draw buffer for the animation.
   * @param draw_buf Initialized draw buffer (ARGB8888).
   */
  void set_draw_buf(lv_draw_buf_t* draw_buf);

  /**
   * @brief Get the underlying animation controller.
   * @return A C++ Animation wrapper for the internal lv_anim_t.
   */
  Animation anim();

  /**
   * @brief Create a Lottie widget.
   * @param parent The parent object.
   * @return A Lottie object.
   */
  static Lottie create(Object& parent);
};

}  // namespace lvgl

#endif  // LV_USE_LOTTIE

#endif  // LVGL_CPP_WIDGETS_LOTTIE_H_
