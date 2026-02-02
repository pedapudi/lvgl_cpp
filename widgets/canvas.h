#ifndef LVGL_CPP_WIDGETS_CANVAS_H_
#define LVGL_CPP_WIDGETS_CANVAS_H_

#include <cstdint>

#include "../core/widget.h"  // IWYU pragma: export
#include "../misc/color.h"   // IWYU pragma: export
#include "../misc/geometry.h"
#include "lvgl.h"  // IWYU pragma: export

#if LV_USE_CANVAS
/**
 * @file canvas.h
 * @brief User Guide:
 * `Canvas` is a powerful widget that allows for custom drawing. It uses a
 * buffer (managed or unmanaged) to store the rendered graphics.
 *
 * Key Features:
 * - **Drawing API**: Wrap LVGL's low-level drawing functions (rects, lines,
 * text, etc.).
 * - **Buffer Access**: Direct pixel manipulation via `set_px` and `get_px`.
 * - **Transformations**: Support for rotation, scaling, and pivots.
 * - **Layer Integration**: Can be used to create custom off-screen buffers.
 *
 * Example:
 * `Canvas(parent).set_buffer(buf, 100, 100,
 * ColorFormat::RGB565).fill_bg(Color::black(), 255);`
 */
namespace lvgl {

class Canvas : public Widget<Canvas> {
 public:
  Canvas();
  explicit Canvas(Object* parent, Ownership ownership = Ownership::Default);
  explicit Canvas(Object& parent);
  explicit Canvas(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Canvas& set_buffer(void* buf, int32_t w, int32_t h, ColorFormat cf);
  Canvas& set_draw_buf(lv_draw_buf_t* draw_buf);
  Canvas& set_px(int32_t x, int32_t y, Color color, Opacity opa);
  Canvas& set_palette(uint8_t index, lv_color32_t color);

  // Copied from Image to maintain API compatibility
  Canvas& set_src(const void* src);
  Canvas& set_offset_x(int32_t x);
  Canvas& set_offset_y(int32_t y);
  Canvas& set_rotation(int32_t angle);
  Canvas& set_pivot(Point pivot);
  Canvas& set_pivot(int32_t x, int32_t y);
  Canvas& set_scale(uint32_t zoom);
  Canvas& set_scale_x(uint32_t zoom);
  Canvas& set_scale_y(uint32_t zoom);
  Canvas& set_blend_mode(BlendMode blend_mode);

  Canvas& set_antialias(bool antialias);

  Canvas& set_inner_align(ImageAlign align);

  lv_draw_buf_t* get_draw_buf();
  lv_color32_t get_px(int32_t x, int32_t y);
  lv_image_dsc_t* get_image();
  const void* get_buf();

  // Image getters
  const void* get_src() const;
  int32_t get_offset_x();
  int32_t get_offset_y();
  int32_t get_rotation();
  Point get_pivot();
  int32_t get_scale();
  int32_t get_scale_x();
  int32_t get_scale_y();
  int32_t get_src_width();
  int32_t get_src_height();
  BlendMode get_blend_mode();
  bool get_antialias();
  ImageAlign get_inner_align();

  void copy_buf(const lv_area_t* canvas_area, lv_draw_buf_t* dest_buf,
                const lv_area_t* dest_area);
  void fill_bg(Color color, lv_opa_t opa);
  void init_layer(lv_layer_t* layer);
  void finish_layer(lv_layer_t* layer);

  // Drawing operations
  Canvas& draw_rect(int32_t x, int32_t y, int32_t w, int32_t h,
                    const lv_draw_rect_dsc_t& dsc);
  Canvas& draw_text(int32_t x, int32_t y, int32_t max_w,
                    const lv_draw_label_dsc_t& dsc, const char* txt);
  Canvas& draw_line(const lv_point_precise_t points[], uint32_t point_cnt,
                    const lv_draw_line_dsc_t& dsc);
  Canvas& draw_image(int32_t x, int32_t y, const lv_draw_image_dsc_t& dsc,
                     const void* src);
  Canvas& draw_arc(int32_t x, int32_t y, uint16_t r, int32_t start_angle,
                   int32_t end_angle, const lv_draw_arc_dsc_t& dsc);

  /**
   * @brief Get the buffer size needed for a canvas.
   * @param w Width in pixels.
   * @param h Height in pixels.
   * @param bpp Bits per pixel.
   * @param stride Stride in bytes.
   * @return Size in bytes.
   */
  static size_t get_buf_size(int32_t w, int32_t h, uint8_t bpp, uint8_t stride);
};

}  // namespace lvgl

#endif  // LV_USE_CANVAS
#endif  // LVGL_CPP_WIDGETS_CANVAS_H_
