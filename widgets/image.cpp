#include "image.h"

#if LV_USE_IMAGE

namespace lvgl {

Image::Image() : Object(lv_image_create(nullptr), Ownership::Managed) {}

Image::Image(Object& parent, Ownership ownership)
    : Object(lv_image_create(parent.raw()), ownership) {}

Image::Image(Object& parent, const void* src) : Image(parent) { set_src(src); }

Image::Image(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

Image& Image::set_src(const void* src) {
  if (obj_) lv_image_set_src(obj_, src);
  return *this;
}

Image& Image::set_offset_x(int32_t x) {
  if (obj_) lv_image_set_offset_x(obj_, x);
  return *this;
}

Image& Image::set_offset_y(int32_t y) {
  if (obj_) lv_image_set_offset_y(obj_, y);
  return *this;
}

Image& Image::set_rotation(int32_t angle) {
  if (obj_) lv_image_set_rotation(obj_, angle);
  return *this;
}

Image& Image::set_pivot(int32_t x, int32_t y) {
  if (obj_) lv_image_set_pivot(obj_, x, y);
  return *this;
}

Image& Image::set_scale(uint32_t zoom) {
  if (obj_) lv_image_set_scale(obj_, zoom);
  return *this;
}

Image& Image::set_scale_x(uint32_t zoom) {
  if (obj_) lv_image_set_scale_x(obj_, zoom);
  return *this;
}

Image& Image::set_scale_y(uint32_t zoom) {
  if (obj_) lv_image_set_scale_y(obj_, zoom);
  return *this;
}

Image& Image::set_blend_mode(lv_blend_mode_t blend_mode) {
  if (obj_) lv_image_set_blend_mode(obj_, blend_mode);
  return *this;
}

Image& Image::set_antialias(bool antialias) {
  if (obj_) lv_image_set_antialias(obj_, antialias);
  return *this;
}

Image& Image::set_inner_align(lv_image_align_t align) {
  if (obj_) lv_image_set_inner_align(obj_, align);
  return *this;
}

Image& Image::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
Image& Image::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Image& Image::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Image& Image::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Image& Image::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Image& Image::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Image& Image::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Image& Image::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

const void* Image::get_src() const {
  return obj_ ? lv_image_get_src(obj_) : nullptr;
}

int32_t Image::get_offset_x() { return obj_ ? lv_image_get_offset_x(obj_) : 0; }

int32_t Image::get_offset_y() { return obj_ ? lv_image_get_offset_y(obj_) : 0; }

int32_t Image::get_rotation() { return obj_ ? lv_image_get_rotation(obj_) : 0; }

void Image::get_pivot(lv_point_t* pivot) {
  if (obj_) lv_image_get_pivot(obj_, pivot);
}

int32_t Image::get_scale() { return obj_ ? lv_image_get_scale(obj_) : 256; }

int32_t Image::get_scale_x() { return obj_ ? lv_image_get_scale_x(obj_) : 256; }

int32_t Image::get_scale_y() { return obj_ ? lv_image_get_scale_y(obj_) : 256; }

int32_t Image::get_src_width() {
  return obj_ ? lv_image_get_src_width(obj_) : 0;
}

int32_t Image::get_src_height() {
  return obj_ ? lv_image_get_src_height(obj_) : 0;
}

lv_blend_mode_t Image::get_blend_mode() {
  return obj_ ? lv_image_get_blend_mode(obj_) : LV_BLEND_MODE_NORMAL;
}

bool Image::get_antialias() {
  return obj_ ? lv_image_get_antialias(obj_) : false;
}

lv_image_align_t Image::get_inner_align() {
  return obj_ ? lv_image_get_inner_align(obj_) : LV_IMAGE_ALIGN_DEFAULT;
}

}  // namespace lvgl

#endif  // LV_USE_IMAGE
