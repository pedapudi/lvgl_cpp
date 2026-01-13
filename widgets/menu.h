#ifndef LVGL_CPP_WIDGETS_MENU_H_
#define LVGL_CPP_WIDGETS_MENU_H_

#include "../core/object.h" // IWYU pragma: export
#include "lvgl.h" // IWYU pragma: export

/**
 * @file menu.h
 * @brief C++ Wrapper for LVGL Menu Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Menu widget(lv_screen_active());
 * widget.center();
 * lvgl::MenuPage main_page = widget.page_create("Main Page");
 * widget.set_page(main_page);
 * ```
 */
namespace lvgl {

class MenuPage : public Object {
public:
  using Object::Object;
};

class MenuCont : public Object {
public:
  using Object::Object;
};

class MenuSection : public Object {
public:
  using Object::Object;
};

class MenuSeparator : public Object {
public:
  using Object::Object;
};

class Menu : public Object {
public:
  /**
   * @brief Create a Menu on the active screen.
   */
  Menu();
  /**
   * @brief Create a Menu with a parent.
   * @param parent The parent object.
   */
  explicit Menu(Object* parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Menu(lv_obj_t* obj);

  MenuPage page_create(const char* title);
  MenuCont cont_create(MenuPage& parent_page);
  MenuSection section_create(MenuPage& parent_page);
  MenuSeparator separator_create(MenuPage& parent_page);

  void set_page(MenuPage& page);
  void set_page_title(lv_obj_t* page, const char* title);
  void set_page_title_static(lv_obj_t* page, const char* title);
  void set_sidebar_page(MenuPage& page);
  void set_mode_header(lv_menu_mode_header_t mode);
  void set_mode_root_back_button(lv_menu_mode_root_back_button_t mode);
  void set_load_page_event(lv_obj_t* obj, MenuPage& page);

  MenuPage get_cur_main_page();
  MenuPage get_cur_sidebar_page();
  lv_obj_t* get_main_header();
  lv_obj_t* get_main_header_back_button();
  lv_obj_t* get_sidebar_header();
  lv_obj_t* get_sidebar_header_back_button();
  bool back_button_is_root(lv_obj_t* obj);
  void clear_history();
};

} // namespace lvgl

#endif // LVGL_CPP_WIDGETS_MENU_H_
