#include "image.h"

namespace lvgl {

Image::Image() : Image((Object*)nullptr) {}

Image::Image(Object *parent)
    : Object(lv_image_create(parent ? parent->raw() : nullptr)) {}

Image::Image(lv_obj_t *obj) : Object(obj) {}

void Image::set_src(const void *src) {
  if (obj_)
    lv_image_set_src(obj_, src);
}

void Image::set_offset_x(int32_t x) {
  if (obj_)
    lv_image_set_offset_x(obj_, x);
}

void Image::set_offset_y(int32_t y) {
  if (obj_)
    lv_image_set_offset_y(obj_, y);
}

void Image::set_rotation(int32_t angle) {
  if (obj_)
    lv_image_set_rotation(obj_, angle);
}

void Image::set_pivot(int32_t x, int32_t y) {
  if (obj_)
    lv_image_set_pivot(obj_, x, y);
}

void Image::set_scale(uint32_t zoom) {
  if (obj_)
    lv_image_set_scale(obj_, zoom);
}

void Image::set_scale_x(uint32_t zoom) {
  if (obj_)
    lv_image_set_scale_x(obj_, zoom);
}

void Image::set_scale_y(uint32_t zoom) {
  if (obj_)
    lv_image_set_scale_y(obj_, zoom);
}

void Image::set_blend_mode(lv_blend_mode_t blend_mode) {
  if (obj_)
    lv_image_set_blend_mode(obj_, blend_mode);
}

void Image::set_antialias(bool antialias) {
  if (obj_)
    lv_image_set_antialias(obj_, antialias);
}

void Image::set_inner_align(lv_image_align_t align) {
  if (obj_)
    lv_image_set_inner_align(obj_, align);
}

const void *Image::get_src() const {
  return obj_ ? lv_image_get_src(obj_) : nullptr;
}

int32_t Image::get_offset_x() { return obj_ ? lv_image_get_offset_x(obj_) : 0; }

int32_t Image::get_offset_y() { return obj_ ? lv_image_get_offset_y(obj_) : 0; }

int32_t Image::get_rotation() { return obj_ ? lv_image_get_rotation(obj_) : 0; }

void Image::get_pivot(lv_point_t *pivot) {
  if (obj_)
    lv_image_get_pivot(obj_, pivot);
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

} // namespace lvgl
