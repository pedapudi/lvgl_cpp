#include "tabview.h"

#if LV_USE_TABVIEW

namespace lvgl {

TabView::TabView() : Widget(lv_tabview_create(nullptr), Ownership::Managed) {}

TabView::TabView(Object& parent, Ownership ownership)
    : Widget(lv_tabview_create(parent.raw()), ownership) {}

TabView::TabView(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

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
