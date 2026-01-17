#include "tabview.h"

#if LV_USE_TABVIEW

namespace lvgl {

TabView::TabView() : Object(lv_tabview_create(nullptr), Ownership::Managed) {}

TabView::TabView(Object& parent, Ownership ownership)
    : Object(lv_tabview_create(parent.raw()), ownership) {}

TabView::TabView(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

TabPage TabView::add_tab(const char* name) {
  return TabPage(obj_ ? lv_tabview_add_tab(obj_, name) : nullptr);
}

TabView& TabView::rename_tab(uint32_t idx, const char* new_name) {
  if (obj_) lv_tabview_rename_tab(obj_, idx, new_name);
  return *this;
}

TabView& TabView::set_active(uint32_t idx, lv_anim_enable_t anim_en) {
  if (obj_) lv_tabview_set_active(obj_, idx, anim_en);
  return *this;
}

TabView& TabView::set_tab_bar_position(lv_dir_t dir) {
  if (obj_) lv_tabview_set_tab_bar_position(obj_, dir);
  return *this;
}

TabView& TabView::set_tab_bar_size(int32_t size) {
  if (obj_) lv_tabview_set_tab_bar_size(obj_, size);
  return *this;
}

TabView& TabView::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
TabView& TabView::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
TabView& TabView::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
TabView& TabView::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
TabView& TabView::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
TabView& TabView::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
TabView& TabView::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
TabView& TabView::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

uint32_t TabView::get_tab_count() {
  return obj_ ? lv_tabview_get_tab_count(obj_) : 0;
}

uint32_t TabView::get_tab_active() {
  return obj_ ? lv_tabview_get_tab_active(obj_) : 0;
}

lv_obj_t* TabView::get_tab_button(int32_t idx) {
  return obj_ ? lv_tabview_get_tab_button(obj_, idx) : nullptr;
}

lv_obj_t* TabView::get_content() {
  return obj_ ? lv_tabview_get_content(obj_) : nullptr;
}

lv_obj_t* TabView::get_tab_bar() {
  return obj_ ? lv_tabview_get_tab_bar(obj_) : nullptr;
}

}  // namespace lvgl

#endif  // LV_USE_TABVIEW
