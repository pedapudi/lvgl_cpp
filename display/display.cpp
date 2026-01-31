#include "display.h"

#include "../core/object.h"

namespace lvgl {

struct DisplayUserData {
  Display::FlushCallback flush_cb;
  Display::FlushWaitCallback flush_wait_cb;
};

static void flush_cb_shim(lv_display_t* disp, const lv_area_t* area,
                          uint8_t* px_map) {
  if (!disp) return;
  auto* user_data =
      static_cast<DisplayUserData*>(lv_display_get_user_data(disp));
  if (user_data && user_data->flush_cb) {
    Display d(disp);
    user_data->flush_cb(&d, area, px_map);
  }
}

static void flush_wait_cb_shim(lv_display_t* disp) {
  if (!disp) return;
  auto* user_data =
      static_cast<DisplayUserData*>(lv_display_get_user_data(disp));
  if (user_data && user_data->flush_wait_cb) {
    Display d(disp);
    user_data->flush_wait_cb(&d);
  }
}

static void display_delete_event_cb(lv_event_t* e) {
  auto* user_data = static_cast<DisplayUserData*>(lv_event_get_user_data(e));
  if (user_data) {
    delete user_data;
    // We should probably null the user data in the display if accessible,
    // but the display is being deleted anyway.
  }
}

Display::Display(lv_display_t* disp) : disp_(disp) {}

Display Display::create(int32_t hor_res, int32_t ver_res) {
  lv_display_t* disp = lv_display_create(hor_res, ver_res);
  // We need to attach user data for callbacks later, or we can just return the
  // wrapper. But to ensure cleanup if we use callbacks, we might want to attach
  // a cleanup event right away if we allocate user_data. However, user_data is
  // allocated only on set_flush_cb. But wait, if we allocate user_data in
  // set_flush_cb, we need the event handler then. So we can leave the event
  // handler attachment to set_flush_cb or do it here if we prepopulate. We'll
  // stick to lazy init in set_flush_cb, BUT we need to make sure the event
  // handler is attached when user_data IS created.
  return Display(disp);
}

Display* Display::get_default() {
  static Display instance(nullptr);
  instance = Display(lv_display_get_default());
  return &instance;
}

void Display::set_default(Display* disp) {
  if (disp && disp->raw()) {
    lv_display_set_default(disp->raw());
  }
}

Display* Display::get_next(Display* prev) {
  // Not implemented for generic iteration returning pointers.
  // Use raw lvgl functions if needed until a better iterator is designed.
  return nullptr;
}

void Display::delete_display() {
  if (disp_) {
    // User data is cleaned up by LV_EVENT_DELETE handler (see set_flush_cb)
    lv_display_delete(disp_);
    disp_ = nullptr;
  }
}

void Display::set_resolution(int32_t hor_res, int32_t ver_res) {
  if (disp_) lv_display_set_resolution(disp_, hor_res, ver_res);
}

void Display::set_physical_resolution(int32_t hor_res, int32_t ver_res) {
  if (disp_) lv_display_set_physical_resolution(disp_, hor_res, ver_res);
}

void Display::set_offset(int32_t x, int32_t y) {
  if (disp_) lv_display_set_offset(disp_, x, y);
}

int32_t Display::get_horizontal_resolution() const {
  return disp_ ? lv_display_get_horizontal_resolution(disp_) : 0;
}

int32_t Display::get_vertical_resolution() const {
  return disp_ ? lv_display_get_vertical_resolution(disp_) : 0;
}

int32_t Display::get_physical_horizontal_resolution() const {
  return disp_ ? lv_display_get_physical_horizontal_resolution(disp_) : 0;
}

int32_t Display::get_physical_vertical_resolution() const {
  return disp_ ? lv_display_get_physical_vertical_resolution(disp_) : 0;
}

int32_t Display::get_offset_x() const {
  return disp_ ? lv_display_get_offset_x(disp_) : 0;
}

int32_t Display::get_offset_y() const {
  return disp_ ? lv_display_get_offset_y(disp_) : 0;
}

int32_t Display::get_original_horizontal_resolution() const {
  return disp_ ? lv_display_get_original_horizontal_resolution(disp_) : 0;
}

int32_t Display::get_original_vertical_resolution() const {
  return disp_ ? lv_display_get_original_vertical_resolution(disp_) : 0;
}

void Display::set_rotation(Rotation rotation) {
  if (disp_)
    lv_display_set_rotation(disp_,
                            static_cast<lv_display_rotation_t>(rotation));
}

Display::Rotation Display::get_rotation() const {
  return disp_ ? static_cast<Rotation>(lv_display_get_rotation(disp_))
               : Rotation::ROT_0;
}

void Display::set_matrix_rotation(bool enable) {
  if (disp_) lv_display_set_matrix_rotation(disp_, enable);
}

bool Display::get_matrix_rotation() const {
  return disp_ ? lv_display_get_matrix_rotation(disp_) : false;
}

void Display::set_dpi(int32_t dpi) {
  if (disp_) lv_display_set_dpi(disp_, dpi);
}

int32_t Display::get_dpi() const {
  return disp_ ? lv_display_get_dpi(disp_) : 0;
}

void Display::set_buffers(void* buf1, void* buf2, uint32_t buf_size,
                          RenderMode render_mode) {
  set_buffers(buf1, buf2, buf_size,
              static_cast<lv_display_render_mode_t>(render_mode));
}

void Display::set_buffers(void* buf1, void* buf2, uint32_t buf_size,
                          lv_display_render_mode_t render_mode) {
  if (disp_) lv_display_set_buffers(disp_, buf1, buf2, buf_size, render_mode);
}

void Display::set_buffers_with_stride(void* buf1, void* buf2, uint32_t buf_size,
                                      uint32_t stride, RenderMode render_mode) {
  set_buffers_with_stride(buf1, buf2, buf_size, stride,
                          static_cast<lv_display_render_mode_t>(render_mode));
}

void Display::set_buffers_with_stride(void* buf1, void* buf2, uint32_t buf_size,
                                      uint32_t stride,
                                      lv_display_render_mode_t render_mode) {
  if (disp_)
    lv_display_set_buffers_with_stride(disp_, buf1, buf2, buf_size, stride,
                                       render_mode);
}

void Display::set_draw_buffers(lv_draw_buf_t* buf1, lv_draw_buf_t* buf2) {
  if (disp_) lv_display_set_draw_buffers(disp_, buf1, buf2);
}

void Display::set_3rd_draw_buffer(lv_draw_buf_t* buf3) {
  if (disp_) lv_display_set_3rd_draw_buffer(disp_, buf3);
}

void Display::set_render_mode(RenderMode render_mode) {
  set_render_mode(static_cast<lv_display_render_mode_t>(render_mode));
}

void Display::set_render_mode(lv_display_render_mode_t render_mode) {
  if (disp_) lv_display_set_render_mode(disp_, render_mode);
}

void Display::set_tile_cnt(uint32_t tile_cnt) {
  if (disp_) lv_display_set_tile_cnt(disp_, tile_cnt);
}

void Display::set_antialiasing(bool en) {
  if (disp_) lv_display_set_antialiasing(disp_, en);
}

uint32_t Display::get_tile_cnt() const {
  return disp_ ? lv_display_get_tile_cnt(disp_) : 0;
}

bool Display::get_antialiasing() const {
  return disp_ ? lv_display_get_antialiasing(disp_) : false;
}

bool Display::is_double_buffered() const {
  return disp_ ? lv_display_is_double_buffered(disp_) : false;
}

uint32_t Display::get_draw_buf_size() const {
  return disp_ ? lv_display_get_draw_buf_size(disp_) : 0;
}

uint32_t Display::get_invalidated_draw_buf_size(uint32_t width,
                                                uint32_t height) const {
  return disp_ ? lv_display_get_invalidated_draw_buf_size(disp_, width, height)
               : 0;
}

void Display::set_color_format(lv_color_format_t color_format) {
  if (disp_) lv_display_set_color_format(disp_, color_format);
}

lv_color_format_t Display::get_color_format() const {
  return disp_ ? lv_display_get_color_format(disp_) : LV_COLOR_FORMAT_UNKNOWN;
}

void Display::set_flush_cb(FlushCallback cb) {
  if (!disp_) return;
  auto* user_data =
      static_cast<DisplayUserData*>(lv_display_get_user_data(disp_));
  if (!user_data) {
    user_data = new DisplayUserData();
    lv_display_set_user_data(disp_, user_data);
    lv_display_add_event_cb(disp_, display_delete_event_cb, LV_EVENT_DELETE,
                            user_data);
  }
  user_data->flush_cb = cb;
  lv_display_set_flush_cb(disp_, cb ? flush_cb_shim : nullptr);
}

void Display::set_flush_wait_cb(FlushWaitCallback cb) {
  if (!disp_) return;
  auto* user_data =
      static_cast<DisplayUserData*>(lv_display_get_user_data(disp_));
  if (!user_data) {
    user_data = new DisplayUserData();
    lv_display_set_user_data(disp_, user_data);
    lv_display_add_event_cb(disp_, display_delete_event_cb, LV_EVENT_DELETE,
                            user_data);
  }
  user_data->flush_wait_cb = cb;
  lv_display_set_flush_wait_cb(disp_, cb ? flush_wait_cb_shim : nullptr);
}

void Display::flush_ready() {
  if (disp_) lv_display_flush_ready(disp_);
}

bool Display::flush_is_last() {
  return disp_ ? lv_display_flush_is_last(disp_) : false;
}

lv_obj_t* Display::get_screen_active() {
  return disp_ ? lv_display_get_screen_active(disp_) : nullptr;
}

lv_obj_t* Display::get_screen_prev() {
  return disp_ ? lv_display_get_screen_prev(disp_) : nullptr;
}

lv_obj_t* Display::get_screen_loading() {
  return disp_ ? lv_display_get_screen_loading(disp_) : nullptr;
}

lv_obj_t* Display::get_layer_top() {
  return disp_ ? lv_display_get_layer_top(disp_) : nullptr;
}

lv_obj_t* Display::get_layer_sys() {
  return disp_ ? lv_display_get_layer_sys(disp_) : nullptr;
}

lv_obj_t* Display::get_layer_bottom() {
  if (!disp_ || disp_ == lv_display_get_default()) return lv_layer_bottom();
  return lv_display_get_layer_bottom(disp_);
}

void Display::load_screen(Object& scr) { lv_screen_load(scr.raw()); }

void Display::load_screen_anim(Object& scr, LoadAnim anim_type, uint32_t time,
                               uint32_t delay, bool auto_del) {
  load_screen_anim(scr, static_cast<lv_screen_load_anim_t>(anim_type), time,
                   delay, auto_del);
}

void Display::load_screen_anim(Object& scr, lv_screen_load_anim_t anim_type,
                               uint32_t time, uint32_t delay, bool auto_del) {
  lv_screen_load_anim(scr.raw(), anim_type, time, delay, auto_del);
}

void Display::set_theme(lv_theme_t* th) {
  if (disp_) lv_display_set_theme(disp_, th);
}

lv_theme_t* Display::get_theme() {
  return disp_ ? lv_display_get_theme(disp_) : nullptr;
}

uint32_t Display::get_inactive_time() const {
  return disp_ ? lv_display_get_inactive_time(disp_) : 0;
}

void Display::trigger_activity() {
  if (disp_) lv_display_trigger_activity(disp_);
}

void Display::enable_invalidation(bool en) {
  if (disp_) lv_display_enable_invalidation(disp_, en);
}

bool Display::is_invalidation_enabled() const {
  return disp_ ? lv_display_is_invalidation_enabled(disp_) : false;
}

void Display::delete_refr_timer() {
  if (disp_) lv_display_delete_refr_timer(disp_);
}

void Display::clear_active_screen() {
  if (disp_) lv_obj_clean(lv_display_get_screen_active(disp_));
}

void Display::auto_configure_buffers(RenderMode mode, bool double_buffer) {
  auto_configure_buffers(static_cast<lv_display_render_mode_t>(mode),
                         double_buffer);
}

void Display::auto_configure_buffers(lv_display_render_mode_t mode,
                                     bool double_buffer) {
  if (!disp_) return;

  uint32_t hor_res = get_horizontal_resolution();
  uint32_t ver_res = get_vertical_resolution();
  lv_color_format_t cf = get_color_format();

  if (hor_res == 0 || ver_res == 0) return;

  uint32_t stride = lv_draw_buf_width_to_stride(hor_res, cf);
  uint32_t buf_size = 0;

  if (mode == LV_DISPLAY_RENDER_MODE_PARTIAL) {
    // Standard recommendation: 1/10 screen size
    buf_size = stride * (ver_res / 10);
  } else {
    // FULL or DIRECT usually requires full screen buffer
    buf_size = stride * ver_res;
  }

  // Allocate buffers using std::vector to manage memory
  buf1_.resize(buf_size);
  if (double_buffer) {
    buf2_.resize(buf_size);
  } else {
    buf2_.clear();
  }

  lv_display_set_buffers(disp_, buf1_.data(),
                         double_buffer ? buf2_.data() : nullptr, buf_size,
                         mode);
}

}  // namespace lvgl
