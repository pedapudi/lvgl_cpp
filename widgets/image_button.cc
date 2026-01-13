#include "image_button.h"

namespace lvgl {

ImageButton::ImageButton() : ImageButton((Object*)nullptr) {}

ImageButton::ImageButton(Object *parent)
    : Object(lv_imagebutton_create(parent ? parent->raw() : nullptr)) {}

ImageButton::ImageButton(lv_obj_t *obj) : Object(obj) {}

void ImageButton::set_src(lv_imagebutton_state_t state, const void *src_left,
                          const void *src_mid, const void *src_right) {
  if (obj_)
    lv_imagebutton_set_src(obj_, state, src_left, src_mid, src_right);
}

void ImageButton::set_state(lv_imagebutton_state_t state) {
  if (obj_)
    lv_imagebutton_set_state(obj_, state);
}

const void *ImageButton::get_src_left(lv_imagebutton_state_t state) {
  return obj_ ? lv_imagebutton_get_src_left(obj_, state) : nullptr;
}

const void *ImageButton::get_src_middle(lv_imagebutton_state_t state) {
  return obj_ ? lv_imagebutton_get_src_middle(obj_, state) : nullptr;
}

const void *ImageButton::get_src_right(lv_imagebutton_state_t state) {
  return obj_ ? lv_imagebutton_get_src_right(obj_, state) : nullptr;
}

} // namespace lvgl
