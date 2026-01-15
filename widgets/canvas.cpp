#include "canvas.h"

#if LV_USE_CANVAS

namespace lvgl {

Canvas::Canvas() : Canvas((Object*)nullptr) {}

Canvas::Canvas(Object* parent, Ownership ownership)
    : Image(lv_canvas_create(parent ? parent->raw() : nullptr)) {}

Canvas::Canvas(lv_obj_t* obj, Ownership ownership) : Image(obj) {}

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

Canvas& Canvas::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
Canvas& Canvas::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Canvas& Canvas::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Canvas& Canvas::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Canvas& Canvas::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Canvas& Canvas::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Canvas& Canvas::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Canvas& Canvas::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
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

}  // namespace lvgl

#endif  // LV_USE_CANVAS
