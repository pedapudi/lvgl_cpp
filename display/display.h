#ifndef LVGL_CPP_DISPLAY_DISPLAY_H_
#define LVGL_CPP_DISPLAY_DISPLAY_H_

#include <functional>

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

namespace lvgl {

class Display {
 public:
  using FlushCallback = std::function<void(Display* disp, const lv_area_t* area,
                                           uint8_t* px_map)>;
  using FlushWaitCallback = std::function<void(Display* disp)>;

  // Usually displays are created by drivers, but we can wrap them.
  explicit Display(lv_display_t* disp);
  virtual ~Display() = default;

  // Creation/Deletion
  static Display create(int32_t hor_res, int32_t ver_res);
  void delete_display();

  static Display* get_default();
  static Display* get_next(Display* prev);

  // Resolution & Physical
  void set_resolution(int32_t hor_res, int32_t ver_res);
  void set_physical_resolution(int32_t hor_res, int32_t ver_res);
  void set_offset(int32_t x, int32_t y);

  int32_t get_horizontal_resolution() const;
  int32_t get_vertical_resolution() const;
  int32_t get_physical_horizontal_resolution() const;
  int32_t get_physical_vertical_resolution() const;
  int32_t get_offset_x() const;
  int32_t get_offset_y() const;
  int32_t get_original_horizontal_resolution() const;
  int32_t get_original_vertical_resolution() const;

  // Rotation
  void set_rotation(lv_display_rotation_t rotation);
  lv_display_rotation_t get_rotation() const;
  void set_matrix_rotation(bool enable);
  bool get_matrix_rotation() const;

  // DPI
  void set_dpi(int32_t dpi);
  int32_t get_dpi() const;

  // Buffers & Rendering
  void set_buffers(void* buf1, void* buf2, uint32_t buf_size,
                   lv_display_render_mode_t render_mode);
  void set_buffers_with_stride(void* buf1, void* buf2, uint32_t buf_size,
                               uint32_t stride,
                               lv_display_render_mode_t render_mode);
  void set_draw_buffers(lv_draw_buf_t* buf1, lv_draw_buf_t* buf2);
  void set_3rd_draw_buffer(lv_draw_buf_t* buf3);
  void set_render_mode(lv_display_render_mode_t render_mode);
  void set_tile_cnt(uint32_t tile_cnt);
  void set_antialiasing(bool en);

  uint32_t get_tile_cnt() const;
  bool get_antialiasing() const;
  bool is_double_buffered() const;
  uint32_t get_draw_buf_size() const;
  uint32_t get_invalidated_draw_buf_size(uint32_t width, uint32_t height) const;

  // Color Format
  void set_color_format(lv_color_format_t color_format);
  lv_color_format_t get_color_format() const;

  // Callbacks
  void set_flush_cb(FlushCallback cb);
  void set_flush_wait_cb(FlushWaitCallback cb);
  void flush_ready();
  bool flush_is_last();

  // Screens & Layers
  lv_obj_t* get_screen_active();
  lv_obj_t* get_screen_prev();
  lv_obj_t* get_screen_loading();
  lv_obj_t* get_layer_top();
  lv_obj_t* get_layer_sys();
  lv_obj_t* get_layer_bottom();

  void load_screen(Object* scr);
  void load_screen_anim(Object* scr, lv_screen_load_anim_t anim_type,
                        uint32_t time, uint32_t delay, bool auto_del);

  // Theme
  void set_theme(lv_theme_t* th);
  lv_theme_t* get_theme();

  // Activity & Invalidation
  uint32_t get_inactive_time() const;
  void trigger_activity();
  void enable_invalidation(bool en);
  bool is_invalidation_enabled() const;

  // Timer
  void delete_refr_timer();

  lv_display_t* raw() const { return disp_; }

 private:
  lv_display_t* disp_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_DISPLAY_DISPLAY_H_
