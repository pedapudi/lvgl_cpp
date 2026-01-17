#ifndef LVGL_CPP_WIDGETS_CANVAS_H_
#define LVGL_CPP_WIDGETS_CANVAS_H_

#include <cstdint>

#include "image.h"  // IWYU pragma: export
#include "lvgl.h"   // IWYU pragma: export

#if LV_USE_CANVAS
/**
 * @file canvas.h
 * @brief C++ Wrapper for LVGL Canvas Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Canvas widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class Canvas : public Image {
 public:
  /**
   * @brief Create a Canvas on the active screen.
   */
  Canvas();
  /**
   * @brief Create a Canvas with a parent.
   * @param parent The parent object.
   */
  explicit Canvas(Object& parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Canvas(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Canvas& set_buffer(void* buf, int32_t w, int32_t h, lv_color_format_t cf);
  Canvas& set_draw_buf(lv_draw_buf_t* draw_buf);
  Canvas& set_px(int32_t x, int32_t y, lv_color_t color, lv_opa_t opa);
  Canvas& set_palette(uint8_t index, lv_color32_t color);

  // Fluent API shadows
  Canvas& set_width(int32_t width);
  Canvas& set_height(int32_t height);
  Canvas& set_size(int32_t width, int32_t height);
  Canvas& align(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Canvas& add_state(lv_state_t state);
  Canvas& remove_state(lv_state_t state);
  Canvas& add_flag(lv_obj_flag_t flag);
  Canvas& remove_flag(lv_obj_flag_t flag);

  lv_draw_buf_t* get_draw_buf();
  lv_color32_t get_px(int32_t x, int32_t y);
  lv_image_dsc_t* get_image();
  const void* get_buf();

  void copy_buf(const lv_area_t* canvas_area, lv_draw_buf_t* dest_buf,
                const lv_area_t* dest_area);
  void fill_bg(lv_color_t color, lv_opa_t opa);
  void init_layer(lv_layer_t* layer);
  void finish_layer(lv_layer_t* layer);
};

/**
 * @file canvas.h
 * @brief C++ Wrapper for LVGL Canvas Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Canvas widget(lv_screen_active());
 * widget.center();
 * ```
 */
}  // namespace lvgl

#endif  // LV_USE_CANVAS
#endif  // LVGL_CPP_WIDGETS_CANVAS_H_
