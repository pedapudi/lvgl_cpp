#include "canvas.h"

#include "../misc/color.h"
#include "lvgl.h"

#if LV_USE_CANVAS

namespace lvgl {

Canvas::Canvas() : Canvas(static_cast<Object*>(nullptr), Ownership::Managed) {}

Canvas::Canvas(Object* parent, Ownership ownership)
    : Widget(lv_canvas_create(parent ? parent->raw() : nullptr), ownership) {}

Canvas::Canvas(Object& parent) : Canvas(&parent) {}

Canvas::Canvas(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Canvas& Canvas::set_buffer(void* buf, int32_t w, int32_t h, ColorFormat cf) {
  if (raw())
    lv_canvas_set_buffer(raw(), buf, w, h, static_cast<lv_color_format_t>(cf));
  return *this;
}

Canvas& Canvas::set_draw_buf(lv_draw_buf_t* draw_buf) {
  if (raw()) lv_canvas_set_draw_buf(raw(), draw_buf);
  return *this;
}

Canvas& Canvas::set_px(int32_t x, int32_t y, Color color, Opacity opa) {
  if (raw()) lv_canvas_set_px(raw(), x, y, color, static_cast<lv_opa_t>(opa));
  return *this;
}

Canvas& Canvas::set_palette(uint8_t index, lv_color32_t color) {
  if (raw()) lv_canvas_set_palette(raw(), index, color);
  return *this;
}

// Copied from Image to maintain API compatibility
Canvas& Canvas::set_src(const void* src) {
  if (raw()) lv_image_set_src(raw(), src);
  return *this;
}

Canvas& Canvas::set_offset_x(int32_t x) {
  if (raw()) lv_image_set_offset_x(raw(), x);
  return *this;
}

Canvas& Canvas::set_offset_y(int32_t y) {
  if (raw()) lv_image_set_offset_y(raw(), y);
  return *this;
}

Canvas& Canvas::set_rotation(int32_t angle) {
  if (raw()) lv_image_set_rotation(raw(), angle);
  return *this;
}

Canvas& Canvas::set_pivot(Point pivot) {
  if (raw()) lv_image_set_pivot(raw(), pivot.x(), pivot.y());
  return *this;
}

Canvas& Canvas::set_pivot(int32_t x, int32_t y) {
  if (raw()) lv_image_set_pivot(raw(), x, y);
  return *this;
}

Canvas& Canvas::set_scale(uint32_t zoom) {
  if (raw()) lv_image_set_scale(raw(), zoom);
  return *this;
}

Canvas& Canvas::set_scale_x(uint32_t zoom) {
  if (raw()) lv_image_set_scale_x(raw(), zoom);
  return *this;
}

Canvas& Canvas::set_scale_y(uint32_t zoom) {
  if (raw()) lv_image_set_scale_y(raw(), zoom);
  return *this;
}

Canvas& Canvas::set_blend_mode(BlendMode blend_mode) {
  if (raw())
    lv_image_set_blend_mode(raw(), static_cast<lv_blend_mode_t>(blend_mode));
  return *this;
}

Canvas& Canvas::set_antialias(bool antialias) {
  if (raw()) lv_image_set_antialias(raw(), antialias);
  return *this;
}

Canvas& Canvas::set_inner_align(ImageAlign align) {
  if (raw())
    lv_image_set_inner_align(raw(), static_cast<lv_image_align_t>(align));
  return *this;
}

lv_draw_buf_t* Canvas::get_draw_buf() {
  return raw() ? lv_canvas_get_draw_buf(raw()) : nullptr;
}

lv_color32_t Canvas::get_px(int32_t x, int32_t y) {
  return raw() ? lv_canvas_get_px(raw(), x, y) : lv_color32_t{0, 0, 0, 0};
}

lv_image_dsc_t* Canvas::get_image() {
  return raw() ? lv_canvas_get_image(raw()) : nullptr;
}

const void* Canvas::get_buf() {
  return raw() ? lv_canvas_get_buf(raw()) : nullptr;
}

void Canvas::copy_buf(const lv_area_t* canvas_area, lv_draw_buf_t* dest_buf,
                      const lv_area_t* dest_area) {
  if (raw()) lv_canvas_copy_buf(raw(), canvas_area, dest_buf, dest_area);
}

void Canvas::fill_bg(Color color, lv_opa_t opa) {
  if (raw()) lv_canvas_fill_bg(raw(), color, opa);
}

void Canvas::init_layer(lv_layer_t* layer) {
  if (raw()) lv_canvas_init_layer(raw(), layer);
}

void Canvas::finish_layer(lv_layer_t* layer) {
  if (raw()) lv_canvas_finish_layer(raw(), layer);
}

// Copied from Image to maintain API compatibility
const void* Canvas::get_src() const {
  return raw() ? lv_image_get_src(raw()) : nullptr;
}

int32_t Canvas::get_offset_x() {
  return raw() ? lv_image_get_offset_x(raw()) : 0;
}

int32_t Canvas::get_offset_y() {
  return raw() ? lv_image_get_offset_y(raw()) : 0;
}

int32_t Canvas::get_rotation() {
  return raw() ? lv_image_get_rotation(raw()) : 0;
}

Point Canvas::get_pivot() {
  lv_point_t p = {0, 0};
  if (raw()) lv_image_get_pivot(raw(), &p);
  return Point(p);
}

int32_t Canvas::get_scale() { return raw() ? lv_image_get_scale(raw()) : 256; }

int32_t Canvas::get_scale_x() {
  return raw() ? lv_image_get_scale_x(raw()) : 256;
}

int32_t Canvas::get_scale_y() {
  return raw() ? lv_image_get_scale_y(raw()) : 256;
}

int32_t Canvas::get_src_width() {
  return raw() ? lv_image_get_src_width(raw()) : 0;
}

int32_t Canvas::get_src_height() {
  return raw() ? lv_image_get_src_height(raw()) : 0;
}

BlendMode Canvas::get_blend_mode() {
  return raw() ? static_cast<BlendMode>(lv_image_get_blend_mode(raw()))
               : BlendMode::Normal;
}

bool Canvas::get_antialias() {
  return raw() ? lv_image_get_antialias(raw()) : false;
}

ImageAlign Canvas::get_inner_align() {
  return raw() ? static_cast<ImageAlign>(lv_image_get_inner_align(raw()))
               : ImageAlign::Default;
}

Canvas& Canvas::draw_rect(int32_t x, int32_t y, int32_t w, int32_t h,
                          const lv_draw_rect_dsc_t& dsc) {
  if (raw()) {
    lv_layer_t layer;
    lv_canvas_init_layer(raw(), &layer);
    lv_area_t coords;
    lv_area_set(&coords, x, y, x + w - 1, y + h - 1);
    lv_draw_rect(&layer, &dsc, &coords);
    lv_canvas_finish_layer(raw(), &layer);
  }
  return *this;
}

Canvas& Canvas::draw_text(int32_t x, int32_t y, int32_t max_w,
                          const lv_draw_label_dsc_t& dsc, const char* txt) {
  if (raw()) {
    lv_layer_t layer;
    lv_canvas_init_layer(raw(), &layer);
    lv_area_t coords;
    // Set a large height for the bounding box; clipping will handle it.
    lv_area_set(&coords, x, y, x + max_w - 1, y + 32767);
    lv_draw_label_dsc_t dsc_copy = dsc;
    dsc_copy.text = txt;
    lv_draw_label(&layer, &dsc_copy, &coords);
    lv_canvas_finish_layer(raw(), &layer);
  }
  return *this;
}

Canvas& Canvas::draw_line(const lv_point_precise_t points[], uint32_t point_cnt,
                          const lv_draw_line_dsc_t& dsc) {
  if (raw() && point_cnt >= 2) {
    lv_layer_t layer;
    lv_canvas_init_layer(raw(), &layer);
    lv_draw_line_dsc_t dsc_copy = dsc;
    dsc_copy.p1 = points[0];
    dsc_copy.p2 = points[1];
    lv_draw_line(&layer, &dsc_copy);
    // Note: LVGL v9 lv_draw_line only takes 2 points.
    // For multiple segments, we would need a loop.
    for (uint32_t i = 1; i < point_cnt - 1; ++i) {
      dsc_copy.p1 = points[i];
      dsc_copy.p2 = points[i + 1];
      lv_draw_line(&layer, &dsc_copy);
    }
    lv_canvas_finish_layer(raw(), &layer);
  }
  return *this;
}

Canvas& Canvas::draw_image(int32_t x, int32_t y, const lv_draw_image_dsc_t& dsc,
                           const void* src) {
  if (raw()) {
    lv_layer_t layer;
    lv_canvas_init_layer(raw(), &layer);
    lv_image_header_t header;
    if (lv_image_decoder_get_info(src, &header) == LV_RESULT_OK) {
      lv_area_t coords;
      lv_area_set(&coords, x, y, x + header.w - 1, y + header.h - 1);
      lv_draw_image_dsc_t dsc_copy = dsc;
      dsc_copy.src = src;
      lv_draw_image(&layer, &dsc_copy, &coords);
    }
    lv_canvas_finish_layer(raw(), &layer);
  }
  return *this;
}

Canvas& Canvas::draw_arc(int32_t x, int32_t y, uint16_t r, int32_t start_angle,
                         int32_t end_angle, const lv_draw_arc_dsc_t& dsc) {
  if (raw()) {
    lv_layer_t layer;
    lv_canvas_init_layer(raw(), &layer);
    lv_draw_arc_dsc_t dsc_copy = dsc;
    dsc_copy.center.x = x;
    dsc_copy.center.y = y;
    dsc_copy.radius = r;
    dsc_copy.start_angle = start_angle;
    dsc_copy.end_angle = end_angle;
    lv_draw_arc(&layer, &dsc_copy);
    lv_canvas_finish_layer(raw(), &layer);
  }
  return *this;
}

size_t Canvas::get_buf_size(int32_t w, int32_t h, uint8_t bpp, uint8_t stride) {
  return lv_canvas_buf_size(w, h, bpp, stride);
}

}  // namespace lvgl

#endif  // LV_USE_CANVAS
