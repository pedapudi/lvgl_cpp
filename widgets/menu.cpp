#include "menu.h"

#if LV_USE_MENU

namespace lvgl {

Menu::Menu() : Menu(static_cast<Object*>(nullptr), Ownership::Managed) {}

Menu::Menu(Object* parent, Ownership ownership)
    : Widget(lv_menu_create(parent ? parent->raw() : nullptr), ownership) {}

Menu::Menu(Object& parent) : Menu(&parent) {}

Menu::Menu(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

MenuPage Menu::page_create(const char* title) {
  return MenuPage(raw() ? lv_menu_page_create(raw(), title) : nullptr);
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

Menu& Menu::set_page(MenuPage& page) {
  if (raw()) lv_menu_set_page(raw(), page.raw());
  return *this;
}

Menu& Menu::set_page_title(MenuPage& page, const char* title) {
  lv_menu_set_page_title(page.raw(), title);
  return *this;
}

Menu& Menu::set_page_title_static(MenuPage& page, const char* title) {
  lv_menu_set_page_title_static(page.raw(), title);
  return *this;
}

Menu& Menu::set_sidebar_page(MenuPage& page) {
  if (raw()) lv_menu_set_sidebar_page(raw(), page.raw());
  return *this;
}

Menu& Menu::set_mode_header(HeaderMode mode) {
  if (raw())
    lv_menu_set_mode_header(raw(), static_cast<lv_menu_mode_header_t>(mode));
  return *this;
}

Menu& Menu::set_mode_root_back_button(RootBackButtonMode mode) {
  if (raw())
    lv_menu_set_mode_root_back_button(
        raw(), static_cast<lv_menu_mode_root_back_button_t>(mode));
  return *this;
}

Menu& Menu::set_load_page_event(Object& obj, MenuPage& page) {
  if (raw()) lv_menu_set_load_page_event(raw(), obj.raw(), page.raw());
  return *this;
}

MenuPage Menu::get_cur_main_page() {
  return MenuPage(raw() ? lv_menu_get_cur_main_page(raw()) : nullptr);
}

MenuPage Menu::get_cur_sidebar_page() {
  return MenuPage(raw() ? lv_menu_get_cur_sidebar_page(raw()) : nullptr);
}

lv_obj_t* Menu::get_main_header() {
  return raw() ? lv_menu_get_main_header(raw()) : nullptr;
}

lv_obj_t* Menu::get_main_header_back_button() {
  return raw() ? lv_menu_get_main_header_back_button(raw()) : nullptr;
}

lv_obj_t* Menu::get_sidebar_header() {
  return raw() ? lv_menu_get_sidebar_header(raw()) : nullptr;
}

lv_obj_t* Menu::get_sidebar_header_back_button() {
  return raw() ? lv_menu_get_sidebar_header_back_button(raw()) : nullptr;
}

bool Menu::back_button_is_root(lv_obj_t* obj) {
  return this->raw() ? lv_menu_back_button_is_root(this->raw(), obj) : false;
}

void Menu::clear_history() {
  if (raw()) lv_menu_clear_history(raw());
}

}  // namespace lvgl

#endif  // LV_USE_MENU
