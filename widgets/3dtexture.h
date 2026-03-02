#ifndef LVGL_CPP_WIDGETS_3DTEXTURE_H_
#define LVGL_CPP_WIDGETS_3DTEXTURE_H_

#include "../core/traits.h"
#include "../core/widget.h"
#include "lvgl.h"

#if LV_USE_3DTEXTURE

namespace lvgl {

class ThreeDTexture;

template <>
struct class_traits<ThreeDTexture> {
  static const lv_obj_class_t* get() { return &lv_3dtexture_class; }
};

/**
 * @brief Wrapper for lv_3dtexture.
 *
 * 3DTexture allows displaying a texture from a 3D graphics backend (e.g.
 * OpenGL).
 */
class ThreeDTexture : public Widget<ThreeDTexture> {
 public:
  ThreeDTexture();
  explicit ThreeDTexture(Object* parent,
                         Ownership ownership = Ownership::Default);
  explicit ThreeDTexture(Object& parent);
  explicit ThreeDTexture(lv_obj_t* obj,
                         Ownership ownership = Ownership::Default);

  /**
   * @brief Set the source texture handle.
   */
  ThreeDTexture& set_src(lv_3dtexture_id_t id);

  /**
   * @brief Set the vertical and horizontal flipping of the texture.
   */
  ThreeDTexture& set_flip(bool h_flip, bool v_flip);
};

}  // namespace lvgl

#endif  // LV_USE_3DTEXTURE
#endif  // LVGL_CPP_WIDGETS_3DTEXTURE_H_
