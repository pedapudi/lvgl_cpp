#include "menu.h"

#if LV_USE_MENU

namespace lvgl {

Menu::Menu() : Menu((Object*)nullptr) {}

Menu::Menu(Object* parent)
    : Object(lv_menu_create(parent ? parent->raw() : nullptr)) {}

Menu::Menu(lv_obj_t* obj) : Object(obj) {}

MenuPage Menu::page_create(const char* title) {
  return MenuPage(obj_ ? lv_menu_page_create(obj_, title) : nullptr);
}

MenuCont Menu::cont_create(MenuPage& parent_page) {
  return MenuCont(lv_menu_cont_create(parent_page.raw()));
}

MenuSection Menu::section_create(MenuPage& parent_page) {
  return MenuSection(lv_menu_section_create(parent_page.raw()));
}

MenuSeparator Menu::separator_create(MenuPage& parent_page) {
  return MenuSeparator(lv_menu_separator_create(parent_page.raw()));
}

void Menu::set_page(MenuPage& page) {
  if (obj_) lv_menu_set_page(obj_, page.raw());
}

void Menu::set_page_title(lv_obj_t* page, const char* title) {
  lv_menu_set_page_title(page, title);
}

void Menu::set_page_title_static(lv_obj_t* page, const char* title) {
  lv_menu_set_page_title_static(page, title);
}

void Menu::set_sidebar_page(MenuPage& page) {
  if (obj_) lv_menu_set_sidebar_page(obj_, page.raw());
}

void Menu::set_mode_header(lv_menu_mode_header_t mode) {
  if (obj_) lv_menu_set_mode_header(obj_, mode);
}

void Menu::set_mode_root_back_button(lv_menu_mode_root_back_button_t mode) {
  if (obj_) lv_menu_set_mode_root_back_button(obj_, mode);
}

void Menu::set_load_page_event(lv_obj_t* obj, MenuPage& page) {
  if (obj_) lv_menu_set_load_page_event(obj_, obj, page.raw());
}

MenuPage Menu::get_cur_main_page() {
  return MenuPage(obj_ ? lv_menu_get_cur_main_page(obj_) : nullptr);
}

MenuPage Menu::get_cur_sidebar_page() {
  return MenuPage(obj_ ? lv_menu_get_cur_sidebar_page(obj_) : nullptr);
}

lv_obj_t* Menu::get_main_header() {
  return obj_ ? lv_menu_get_main_header(obj_) : nullptr;
}

lv_obj_t* Menu::get_main_header_back_button() {
  return obj_ ? lv_menu_get_main_header_back_button(obj_) : nullptr;
}

lv_obj_t* Menu::get_sidebar_header() {
  return obj_ ? lv_menu_get_sidebar_header(obj_) : nullptr;
}

lv_obj_t* Menu::get_sidebar_header_back_button() {
  return obj_ ? lv_menu_get_sidebar_header_back_button(obj_) : nullptr;
}

bool Menu::back_button_is_root(lv_obj_t* obj) {
  return this->obj_ ? lv_menu_back_button_is_root(this->obj_, obj) : false;
}

void Menu::clear_history() {
  if (obj_) lv_menu_clear_history(obj_);
}

}  // namespace lvgl

#endif // LV_USE_MENU
