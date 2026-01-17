#ifndef LVGL_CPP_WIDGETS_TABVIEW_H_
#define LVGL_CPP_WIDGETS_TABVIEW_H_

#include <cstdint>

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_TABVIEW
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
  explicit TabView(Object& parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit TabView(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  TabPage add_tab(const char* name);
  TabView& rename_tab(uint32_t idx, const char* new_name);
  TabView& set_active(uint32_t idx, lv_anim_enable_t anim_en);
  TabView& set_tab_bar_position(lv_dir_t dir);
  TabView& set_tab_bar_size(int32_t size);

  // Fluent API shadows
  TabView& set_width(int32_t width);
  TabView& set_height(int32_t height);
  TabView& set_size(int32_t width, int32_t height);
  TabView& align(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  TabView& add_state(lv_state_t state);
  TabView& remove_state(lv_state_t state);
  TabView& add_flag(lv_obj_flag_t flag);
  TabView& remove_flag(lv_obj_flag_t flag);

  uint32_t get_tab_count();
  uint32_t get_tab_active();
  lv_obj_t* get_tab_button(int32_t idx);
  lv_obj_t* get_content();
  lv_obj_t* get_tab_bar();
};

}  // namespace lvgl

#endif  // LV_USE_TABVIEW
#endif  // LVGL_CPP_WIDGETS_TABVIEW_H_
