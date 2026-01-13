#include "canvas.h"

namespace lvgl {

Canvas::Canvas() : Canvas((Object*)nullptr) {}

Canvas::Canvas(Object* parent)
    : Image(lv_canvas_create(parent ? parent->raw() : nullptr)) {}

Canvas::Canvas(lv_obj_t* obj) : Image(obj) {}

void Canvas::set_buffer(void* buf, int32_t w, int32_t h, lv_color_format_t cf) {
  if (obj_)
    lv_canvas_set_buffer(obj_, buf, w, h, cf);
}

void Canvas::set_draw_buf(lv_draw_buf_t* draw_buf) {
  if (obj_)
    lv_canvas_set_draw_buf(obj_, draw_buf);
}

void Canvas::set_px(int32_t x, int32_t y, lv_color_t color, lv_opa_t opa) {
  if (obj_)
    lv_canvas_set_px(obj_, x, y, color, opa);
}

void Canvas::set_palette(uint8_t index, lv_color32_t color) {
  if (obj_)
    lv_canvas_set_palette(obj_, index, color);
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
  if (obj_)
    lv_canvas_copy_buf(obj_, canvas_area, dest_buf, dest_area);
}

void Canvas::fill_bg(lv_color_t color, lv_opa_t opa) {
  if (obj_)
    lv_canvas_fill_bg(obj_, color, opa);
}

void Canvas::init_layer(lv_layer_t* layer) {
  if (obj_)
    lv_canvas_init_layer(obj_, layer);
}

void Canvas::finish_layer(lv_layer_t* layer) {
  if (obj_)
    lv_canvas_finish_layer(obj_, layer);
}

} // namespace lvgl
