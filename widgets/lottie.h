#ifndef LVGL_CPP_WIDGETS_LOTTIE_H_
#define LVGL_CPP_WIDGETS_LOTTIE_H_

#include "../core/widget.h"
#include "lvgl.h"

#if LV_USE_LOTTIE

namespace lvgl {

class Lottie : public Widget<Lottie> {
 public:
  Lottie();
  explicit Lottie(Object* parent, Ownership ownership = Ownership::Default);
  explicit Lottie(Object& parent);
  explicit Lottie(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief Set a buffer for the animation. It also defines the size of the
   * animation.
   * @param w width of the animation and buffer
   * @param h height of the animation and buffer
   * @param buf a static buffer with `width x height x 4` byte size
   */
  Lottie& set_buffer(int32_t w, int32_t h, void* buf);

  /**
   * @brief Set a draw buffer for the animation. It also defines the size of the
   * animation.
   * @param draw_buf an initialized draw buffer with ARGB8888 color format
   */
  Lottie& set_draw_buf(lv_draw_buf_t* draw_buf);

  /**
   * @brief Set the source for the animation as an array.
   * @param src the lottie animation converted to a null terminated array
   * @param src_size size of the source array in bytes
   */
  Lottie& set_src_data(const void* src, size_t src_size);

  /**
   * @brief Set the source for the animation as a path.
   * @param src path to a json file
   */
  Lottie& set_src_file(const char* src);

  /**
   * @brief Get the underlying LVGL animation object.
   * @return Handle to the internal lv_anim_t
   */
  lv_anim_t* get_anim();
};

}  // namespace lvgl

#endif  // LV_USE_LOTTIE
#endif  // LVGL_CPP_WIDGETS_LOTTIE_H_
