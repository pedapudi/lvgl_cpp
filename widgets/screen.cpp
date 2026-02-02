#include "screen.h"

namespace lvgl {

Screen::Screen() : Widget(static_cast<Object*>(nullptr)) {
  // lv_obj_create(NULL) creates a screen
}

Screen::Screen(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

void Screen::load() {
  if (raw()) lv_screen_load(raw());
}

void Screen::load_anim(ScreenLoadAnim anim_type, uint32_t time, uint32_t delay,
                       bool auto_del) {
  if (raw())
    lv_screen_load_anim(raw(), static_cast<lv_screen_load_anim_t>(anim_type),
                        time, delay, auto_del);
}

Screen Screen::active() {
  return Screen(lv_screen_active(), Ownership::Unmanaged);
}

Screen Screen::top() { return Screen(lv_layer_top(), Ownership::Unmanaged); }

Screen Screen::sys() { return Screen(lv_layer_sys(), Ownership::Unmanaged); }

}  // namespace lvgl
