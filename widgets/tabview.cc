#include "tabview.h"

namespace lvgl {

TabView::TabView() : TabView((Object *)nullptr) {}

TabView::TabView(Object *parent)
    : Object(lv_tabview_create(parent ? parent->raw() : nullptr)) {}

TabView::TabView(lv_obj_t *obj) : Object(obj) {}

TabPage TabView::add_tab(const char *name) {
  return TabPage(obj_ ? lv_tabview_add_tab(obj_, name) : nullptr);
}

void TabView::rename_tab(uint32_t idx, const char *new_name) {
  if (obj_)
    lv_tabview_rename_tab(obj_, idx, new_name);
}

void TabView::set_active(uint32_t idx, lv_anim_enable_t anim_en) {
  if (obj_)
    lv_tabview_set_active(obj_, idx, anim_en);
}

void TabView::set_tab_bar_position(lv_dir_t dir) {
  if (obj_)
    lv_tabview_set_tab_bar_position(obj_, dir);
}

void TabView::set_tab_bar_size(int32_t size) {
  if (obj_)
    lv_tabview_set_tab_bar_size(obj_, size);
}

uint32_t TabView::get_tab_count() {
  return obj_ ? lv_tabview_get_tab_count(obj_) : 0;
}

uint32_t TabView::get_tab_active() {
  return obj_ ? lv_tabview_get_tab_active(obj_) : 0;
}

lv_obj_t *TabView::get_tab_button(int32_t idx) {
  return obj_ ? lv_tabview_get_tab_button(obj_, idx) : nullptr;
}

lv_obj_t *TabView::get_content() {
  return obj_ ? lv_tabview_get_content(obj_) : nullptr;
}

lv_obj_t *TabView::get_tab_bar() {
  return obj_ ? lv_tabview_get_tab_bar(obj_) : nullptr;
}

} // namespace lvgl
