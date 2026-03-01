#include "3dtexture.h"

#if LV_USE_3DTEXTURE

namespace lvgl {

ThreeDTexture::ThreeDTexture()
    : ThreeDTexture(static_cast<Object*>(nullptr), Ownership::Managed) {}

ThreeDTexture::ThreeDTexture(Object* parent, Ownership ownership)
    : Widget(lv_3dtexture_create(parent ? parent->raw() : nullptr), ownership) {
}

ThreeDTexture::ThreeDTexture(Object& parent) : ThreeDTexture(&parent) {}

ThreeDTexture::ThreeDTexture(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

ThreeDTexture& ThreeDTexture::set_src(lv_3dtexture_id_t id) {
  if (raw()) lv_3dtexture_set_src(raw(), id);
  return *this;
}

ThreeDTexture& ThreeDTexture::set_flip(bool h_flip, bool v_flip) {
  if (raw()) lv_3dtexture_set_flip(raw(), h_flip, v_flip);
  return *this;
}

}  // namespace lvgl

#endif  // LV_USE_3DTEXTURE
