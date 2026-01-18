#include "canvas.h"

#if LV_USE_CANVAS

namespace lvgl {

Canvas::Canvas() : Canvas(static_cast<Object*>(nullptr), Ownership::Managed) {}

Canvas::Canvas(Object* parent, Ownership ownership)
    : Widget(lv_canvas_create(parent ? parent->raw() : nullptr), ownership) {}

Canvas::Canvas(Object& parent) : Canvas(&parent) {}

Canvas::Canvas(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Canvas& Canvas::set_buffer(void* buf, int32_t w, int32_t h,
                           lv_color_format_t cf) {
  if (obj_) lv_canvas_set_buffer(obj_, buf, w, h, cf);
  return *this;
}

Canvas& Canvas::set_draw_buf(lv_draw_buf_t* draw_buf) {
  if (obj_) lv_canvas_set_draw_buf(obj_, draw_buf);
  return *this;
}

Canvas& Canvas::set_px(int32_t x, int32_t y, lv_color_t color, lv_opa_t opa) {
  if (obj_) lv_canvas_set_px(obj_, x, y, color, opa);
  return *this;
}

Canvas& Canvas::set_palette(uint8_t index, lv_color32_t color) {
  if (obj_) lv_canvas_set_palette(obj_, index, color);
  return *this;
}

// Copied from Image to maintain API compatibility
Canvas& Canvas::set_src(const void* src) {
  if (obj_) lv_image_set_src(obj_, src);
  return *this;
}

Canvas& Canvas::set_offset_x(int32_t x) {
  if (obj_) lv_image_set_offset_x(obj_, x);
  return *this;
}

Canvas& Canvas::set_offset_y(int32_t y) {
  if (obj_) lv_image_set_offset_y(obj_, y);
  return *this;
}

Canvas& Canvas::set_rotation(int32_t angle) {
  if (obj_) lv_image_set_rotation(obj_, angle);
  return *this;
}

Canvas& Canvas::set_pivot(int32_t x, int32_t y) {
  if (obj_) lv_image_set_pivot(obj_, x, y);
  return *this;
}

Canvas& Canvas::set_scale(uint32_t zoom) {
  if (obj_) lv_image_set_scale(obj_, zoom);
  return *this;
}

Canvas& Canvas::set_scale_x(uint32_t zoom) {
  if (obj_) lv_image_set_scale_x(obj_, zoom);
  return *this;
}

Canvas& Canvas::set_scale_y(uint32_t zoom) {
  if (obj_) lv_image_set_scale_y(obj_, zoom);
  return *this;
}

Canvas& Canvas::set_blend_mode(lv_blend_mode_t blend_mode) {
  if (obj_) lv_image_set_blend_mode(obj_, blend_mode);
  return *this;
}

Canvas& Canvas::set_antialias(bool antialias) {
  if (obj_) lv_image_set_antialias(obj_, antialias);
  return *this;
}

Canvas& Canvas::set_inner_align(lv_image_align_t align) {
  if (obj_) lv_image_set_inner_align(obj_, align);
  return *this;
}

lv_draw_buf_t* Canvas::get_draw_buf() {
  return obj_ ? lv_canvas_get_draw_buf(obj_) : nullptr;
}

lv_color32_t Canvas::get_px(int32_t x, int32_t y) {
  return obj_ ? lv_canvas_get_px(obj_, x, y) : lv_color32_t{0, 0, 0, 0};
}

lv_image_dsc_t* Canvas::get_image() {
  return obj_ ? lv_canvas_get_image(obj_) : nullptr;
}

const void* Canvas::get_buf() {
  return obj_ ? lv_canvas_get_buf(obj_) : nullptr;
}

void Canvas::copy_buf(const lv_area_t* canvas_area, lv_draw_buf_t* dest_buf,
                      const lv_area_t* dest_area) {
  if (obj_) lv_canvas_copy_buf(obj_, canvas_area, dest_buf, dest_area);
}

void Canvas::fill_bg(lv_color_t color, lv_opa_t opa) {
  if (obj_) lv_canvas_fill_bg(obj_, color, opa);
}

void Canvas::init_layer(lv_layer_t* layer) {
  if (obj_) lv_canvas_init_layer(obj_, layer);
}

void Canvas::finish_layer(lv_layer_t* layer) {
  if (obj_) lv_canvas_finish_layer(obj_, layer);
}

// Copied from Image to maintain API compatibility
const void* Canvas::get_src() const {
  return obj_ ? lv_image_get_src(obj_) : nullptr;
}

int32_t Canvas::get_offset_x() {
  return obj_ ? lv_image_get_offset_x(obj_) : 0;
}

int32_t Canvas::get_offset_y() {
  return obj_ ? lv_image_get_offset_y(obj_) : 0;
}

int32_t Canvas::get_rotation() {
  return obj_ ? lv_image_get_rotation(obj_) : 0;
}

void Canvas::get_pivot(lv_point_t* pivot) {
  if (obj_) lv_image_get_pivot(obj_, pivot);
}

int32_t Canvas::get_scale() { return obj_ ? lv_image_get_scale(obj_) : 256; }

int32_t Canvas::get_scale_x() {
  return obj_ ? lv_image_get_scale_x(obj_) : 256;
}

int32_t Canvas::get_scale_y() {
  return obj_ ? lv_image_get_scale_y(obj_) : 256;
}

int32_t Canvas::get_src_width() {
  return obj_ ? lv_image_get_src_width(obj_) : 0;
}

int32_t Canvas::get_src_height() {
  return obj_ ? lv_image_get_src_height(obj_) : 0;
}

lv_blend_mode_t Canvas::get_blend_mode() {
  return obj_ ? lv_image_get_blend_mode(obj_) : LV_BLEND_MODE_NORMAL;
}

bool Canvas::get_antialias() {
  return obj_ ? lv_image_get_antialias(obj_) : false;
}

lv_image_align_t Canvas::get_inner_align() {
  return obj_ ? lv_image_get_inner_align(obj_) : LV_IMAGE_ALIGN_DEFAULT;
}

}  // namespace lvgl

#endif  // LV_USE_CANVAS
