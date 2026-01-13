#ifndef LVGL_CPP_DISPLAY_DISPLAY_H_
#define LVGL_CPP_DISPLAY_DISPLAY_H_

#include "../core/object.h" // IWYU pragma: export
#include "lvgl.h" // IWYU pragma: export

namespace lvgl {

class Display {
public:
  // Usually displays are created by drivers, but we can wrap them.
  explicit Display(lv_display_t *disp);

  static Display *get_default();
  static Display *get_next(Display *prev);

  void set_resolution(int32_t hor_res, int32_t ver_res);
  void set_physical_resolution(int32_t hor_res, int32_t ver_res);
  void set_offset(int32_t x, int32_t y);
  void set_rotation(lv_display_rotation_t rotation);
  void set_dpi(int32_t dpi);
  void set_render_mode(lv_display_render_mode_t render_mode);

  int32_t get_horizontal_resolution() const;
  int32_t get_vertical_resolution() const;
  int32_t get_dpi() const;
  lv_display_rotation_t get_rotation();

  lv_obj_t *get_screen_active();
  lv_obj_t *get_screen_prev();
  lv_obj_t *get_layer_top();
  lv_obj_t *get_layer_sys();
  lv_obj_t *get_layer_bottom();

  void load_screen(Object *scr);
  void load_screen_anim(Object *scr, lv_screen_load_anim_t anim_type,
                        uint32_t time, uint32_t delay, bool auto_del);

  lv_display_t *raw() const { return disp_; }

private:
  lv_display_t *disp_;
};

} // namespace lvgl

#endif // LVGL_CPP_DISPLAY_DISPLAY_H_
