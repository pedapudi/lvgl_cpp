#include "tabview.h"

#if LV_USE_TABVIEW

namespace lvgl {

TabView::TabView()
    : TabView(static_cast<Object*>(nullptr), Ownership::Managed) {}

TabView::TabView(Object* parent, Ownership ownership)
    : Widget(lv_tabview_create(parent ? parent->raw() : nullptr), ownership) {}

TabView::TabView(Object& parent) : TabView(&parent) {}

TabView::TabView(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

TabPage TabView::add_tab(const char* name) {
  return TabPage(raw() ? lv_tabview_add_tab(raw(), name) : nullptr,
                 Ownership::Unmanaged);
}

TabView& TabView::rename_tab(uint32_t idx, const char* new_name) {
  if (raw()) lv_tabview_rename_tab(raw(), idx, new_name);
  return *this;
}

TabView& TabView::set_active(uint32_t idx, AnimEnable anim) {
  if (raw())
    lv_tabview_set_active(raw(), idx, static_cast<lv_anim_enable_t>(anim));
  return *this;
}

TabView& TabView::set_tab_bar_position(Dir dir) {
  if (raw()) lv_tabview_set_tab_bar_position(raw(), static_cast<lv_dir_t>(dir));
  return *this;
}

TabView& TabView::set_tab_bar_size(int32_t size) {
  if (raw()) lv_tabview_set_tab_bar_size(raw(), size);
  return *this;
}

uint32_t TabView::get_tab_count() {
  return raw() ? lv_tabview_get_tab_count(raw()) : 0;
}

uint32_t TabView::get_tab_active() {
  return raw() ? lv_tabview_get_tab_active(raw()) : 0;
}

lv_obj_t* TabView::get_tab_button(int32_t idx) {
  return raw() ? lv_tabview_get_tab_button(raw(), idx) : nullptr;
}

lv_obj_t* TabView::get_content() {
  return raw() ? lv_tabview_get_content(raw()) : nullptr;
}

lv_obj_t* TabView::get_tab_bar() {
  return raw() ? lv_tabview_get_tab_bar(raw()) : nullptr;
}

}  // namespace lvgl

#endif  // LV_USE_TABVIEW
