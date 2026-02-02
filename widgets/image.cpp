#include "image.h"

#if LV_USE_IMAGE

namespace lvgl {

Image::Image() : Image(static_cast<Object*>(nullptr), Ownership::Managed) {}

Image::Image(Object* parent, Ownership ownership)
    : Widget(lv_image_create(parent ? parent->raw() : nullptr), ownership) {}

Image::Image(Object& parent) : Image(&parent) {}

Image::Image(Object& parent, const void* src) : Image(parent) { set_src(src); }

Image::Image(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Image& Image::set_src(const void* src) {
  if (raw()) lv_image_set_src(raw(), src);
  return *this;
}

Image& Image::set_src(const std::string& path) {
  // lv_image_set_src copies the string if it's a file path (verified in
  // source).
  if (raw()) lv_image_set_src(raw(), path.c_str());
  return *this;
}

Image& Image::set_src(const ImageDescriptor& dsc) {
  if (raw()) lv_image_set_src(raw(), dsc.raw());
  return *this;
}

Image& Image::set_offset_x(int32_t x) {
  if (raw()) lv_image_set_offset_x(raw(), x);
  return *this;
}

Image& Image::set_offset_y(int32_t y) {
  if (raw()) lv_image_set_offset_y(raw(), y);
  return *this;
}

Image& Image::set_rotation(int32_t angle) {
  if (raw()) lv_image_set_rotation(raw(), angle);
  return *this;
}

Image& Image::set_pivot(Point pivot) {
  if (raw()) lv_image_set_pivot(raw(), pivot.x(), pivot.y());
  return *this;
}

Image& Image::set_scale(uint32_t zoom) {
  if (raw()) lv_image_set_scale(raw(), zoom);
  return *this;
}

Image& Image::set_scale_x(uint32_t zoom) {
  if (raw()) lv_image_set_scale_x(raw(), zoom);
  return *this;
}

Image& Image::set_scale_y(uint32_t zoom) {
  if (raw()) lv_image_set_scale_y(raw(), zoom);
  return *this;
}

Image& Image::set_blend_mode(BlendMode blend_mode) {
  if (raw())
    lv_image_set_blend_mode(raw(), static_cast<lv_blend_mode_t>(blend_mode));
  return *this;
}

Image& Image::set_antialias(bool antialias) {
  if (raw()) lv_image_set_antialias(raw(), antialias);
  return *this;
}

Image& Image::set_inner_align(ImageAlign align) {
  if (raw())
    lv_image_set_inner_align(raw(), static_cast<lv_image_align_t>(align));
  return *this;
}

const void* Image::get_src() const {
  return raw() ? lv_image_get_src(raw()) : nullptr;
}

int32_t Image::get_offset_x() {
  return raw() ? lv_image_get_offset_x(raw()) : 0;
}

int32_t Image::get_offset_y() {
  return raw() ? lv_image_get_offset_y(raw()) : 0;
}

int32_t Image::get_rotation() {
  return raw() ? lv_image_get_rotation(raw()) : 0;
}

Point Image::get_pivot() {
  lv_point_t p = {0, 0};
  if (raw()) lv_image_get_pivot(raw(), &p);
  return Point(p);
}

int32_t Image::get_scale() { return raw() ? lv_image_get_scale(raw()) : 256; }

int32_t Image::get_scale_x() {
  return raw() ? lv_image_get_scale_x(raw()) : 256;
}

int32_t Image::get_scale_y() {
  return raw() ? lv_image_get_scale_y(raw()) : 256;
}

int32_t Image::get_src_width() {
  return raw() ? lv_image_get_src_width(raw()) : 0;
}

int32_t Image::get_src_height() {
  return raw() ? lv_image_get_src_height(raw()) : 0;
}

BlendMode Image::get_blend_mode() {
  return raw() ? static_cast<BlendMode>(lv_image_get_blend_mode(raw()))
               : BlendMode::Normal;
}

bool Image::get_antialias() {
  return raw() ? lv_image_get_antialias(raw()) : false;
}

ImageAlign Image::get_inner_align() {
  return raw() ? static_cast<ImageAlign>(lv_image_get_inner_align(raw()))
               : ImageAlign::Default;
}

}  // namespace lvgl

#endif  // LV_USE_IMAGE
