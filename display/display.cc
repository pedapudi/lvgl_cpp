#include "display.h"

namespace lvgl {

Display::Display(lv_display_t* disp) : disp_(disp) {}

Display* Display::get_default() {
  static Display default_display(lv_display_get_default());
  // Update internal pointer as default display might change
  // But this static object approach is flawed if default changes.
  // Better to just create a temporary wrapper or handle it differently.
  // For now, returning a new instance on heap might be safer if user manages
  // it, but standard practice is not to. I will just return a pointer to a
  // static instance that updates its pointer.
  static Display instance(nullptr);
  instance = Display(lv_display_get_default());
  return &instance;
}

void Display::set_resolution(int32_t hor_res, int32_t ver_res) {
  if (disp_)
    lv_display_set_resolution(disp_, hor_res, ver_res);
}

void Display::set_physical_resolution(int32_t hor_res, int32_t ver_res) {
  if (disp_)
    lv_display_set_physical_resolution(disp_, hor_res, ver_res);
}

void Display::set_offset(int32_t x, int32_t y) {
  if (disp_)
    lv_display_set_offset(disp_, x, y);
}

void Display::set_rotation(lv_display_rotation_t rotation) {
  if (disp_)
    lv_display_set_rotation(disp_, rotation);
}

void Display::set_dpi(int32_t dpi) {
  if (disp_)
    lv_display_set_dpi(disp_, dpi);
}

void Display::set_render_mode(lv_display_render_mode_t render_mode) {
  if (disp_)
    lv_display_set_render_mode(disp_, render_mode);
}

int32_t Display::get_horizontal_resolution() const {
  return disp_ ? lv_display_get_horizontal_resolution(disp_) : 0;
}

int32_t Display::get_vertical_resolution() const {
  return disp_ ? lv_display_get_vertical_resolution(disp_) : 0;
}

int32_t Display::get_dpi() const {
  return disp_ ? lv_display_get_dpi(disp_) : 0;
}

lv_display_rotation_t Display::get_rotation() {
  return disp_ ? lv_display_get_rotation(disp_) : LV_DISPLAY_ROTATION_0;
}

lv_obj_t* Display::get_screen_active() {
  return disp_ ? lv_display_get_screen_active(disp_) : nullptr;
}

lv_obj_t* Display::get_screen_prev() {
  return disp_ ? lv_display_get_screen_prev(disp_) : nullptr;
}

lv_obj_t* Display::get_layer_top() {
  return disp_ ? lv_display_get_layer_top(disp_) : nullptr;
}

lv_obj_t* Display::get_layer_sys() {
  return disp_ ? lv_display_get_layer_sys(disp_) : nullptr;
}

lv_obj_t* Display::get_layer_bottom() {
  return disp_ ? lv_display_get_layer_bottom(disp_) : nullptr;
}

void Display::load_screen(Object* scr) {
  if (scr)
    lv_screen_load(scr->raw());
}

void Display::load_screen_anim(Object* scr, lv_screen_load_anim_t anim_type,
                               uint32_t time, uint32_t delay, bool auto_del) {
  if (scr)
    lv_screen_load_anim(scr->raw(), anim_type, time, delay, auto_del);
}

} // namespace lvgl
