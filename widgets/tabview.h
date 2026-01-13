#ifndef LVGL_CPP_WIDGETS_TABVIEW_H_
#define LVGL_CPP_WIDGETS_TABVIEW_H_

#include "../core/object.h"
#include "lvgl.h"

/**
 * @file tabview.h
 * @brief C++ Wrapper for LVGL TabView Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::TabView widget(lv_screen_active());
 * widget.center();
 * lvgl::TabPage tab = widget.add_tab("Tab 1");
 * ```
 */

namespace lvgl {

/**
 * @brief Represents a single tab page container.
 */
class TabPage : public Object {
public:
  using Object::Object;
};

/**
 * @brief Wrapper for lv_tabview.
 */
class TabView : public Object {
public:
  /**
   * @brief Create a TabView on the active screen.
   */
  TabView();
  /**
   * @brief Create a TabView with a parent.
   * @param parent The parent object.
   */
  explicit TabView(Object *parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit TabView(lv_obj_t *obj);

  TabPage add_tab(const char *name);
  void rename_tab(uint32_t idx, const char *new_name);
  void set_active(uint32_t idx, lv_anim_enable_t anim_en);
  void set_tab_bar_position(lv_dir_t dir);
  void set_tab_bar_size(int32_t size);

  uint32_t get_tab_count();
  uint32_t get_tab_active();
  lv_obj_t *get_tab_button(int32_t idx);
  lv_obj_t *get_content();
  lv_obj_t *get_tab_bar();
};

} // namespace lvgl

#endif // LVGL_CPP_WIDGETS_TABVIEW_H_
