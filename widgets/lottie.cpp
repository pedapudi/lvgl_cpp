#include "lottie.h"

#if LV_USE_LOTTIE

namespace lvgl {

Lottie Lottie::create(Object& parent) {
  lv_obj_t* obj = lv_lottie_create(parent.raw());
  return Lottie(obj, Ownership::Managed);
}

void Lottie::set_src(const void* src, size_t src_size) {
  lv_lottie_set_src_data(raw(), src, src_size);
}

void Lottie::set_src(const char* path) { lv_lottie_set_src_file(raw(), path); }

void Lottie::set_buffer(int32_t w, int32_t h, void* buf) {
  lv_lottie_set_buffer(raw(), w, h, buf);
}

void Lottie::set_draw_buf(lv_draw_buf_t* draw_buf) {
  lv_lottie_set_draw_buf(raw(), draw_buf);
}

void Lottie::set_draw_buf(const DrawBuf& draw_buf) {
  lv_lottie_set_draw_buf(raw(), draw_buf.raw());
}

Animation Lottie::anim() {
  lv_anim_t* a = lv_lottie_get_anim(raw());
  return Animation(a);
}

Lottie Lottie::create(Object& parent, int32_t w, int32_t h) {
  Lottie l = create(parent);
  // Allocate a draw buffer. Note: For now, we don't manage the DrawBuf
  // lifetime here in a special way because lv_lottie_set_draw_buf
  // doesn't take ownership.
  // Actually, we should probably return a wrapper that owns the buffer
  // or use the Canvas-style buffer management.
  // LVGL v9's lv_draw_buf_create allocates the buffer on the heap.
  lv_draw_buf_t* db = lv_draw_buf_create(w, h, LV_COLOR_FORMAT_ARGB8888, 0);
  lv_lottie_set_draw_buf(l.raw(), db);
  // Mark for automatic destruction when widget is deleted?
  // lv_lottie doesn't have a hook for this.
  // Let's use lv_obj_add_event_cb to cleanup.
  lv_obj_add_event_cb(
      l.raw(),
      [](lv_event_t* e) {
        lv_draw_buf_t* db = (lv_draw_buf_t*)lv_event_get_user_data(e);
        if (db) lv_draw_buf_destroy(db);
      },
      LV_EVENT_DELETE, db);

  return l;
}

}  // namespace lvgl

#endif  // LV_USE_LOTTIE
