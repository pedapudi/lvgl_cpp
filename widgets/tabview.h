#ifndef LVGL_CPP_WIDGETS_TABVIEW_H_
#define LVGL_CPP_WIDGETS_TABVIEW_H_

#include <cstdint>

#include "../core/widget.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_TABVIEW

namespace lvgl {

/**
 * @brief Represents a single tab page container.
 * Inherits from Widget<TabPage> to allow fluent API on the page itself.
 */
class TabPage : public Widget<TabPage> {
 public:
  using Widget::Widget;
};

/**
 * @brief Wrapper for lv_tabview.
 */
class TabView : public Widget<TabView> {
 public:
  TabView();
  explicit TabView(Object* parent, Ownership ownership = Ownership::Default);
  explicit TabView(Object& parent);
  explicit TabView(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief Add a new tab.
   * @param name The title of the tab.
   * @return A TabPage wrapper with Ownership::Unmanaged.
   *         The page is owned by the TabView.
   */
  TabPage add_tab(const char* name);

  TabView& rename_tab(uint32_t idx, const char* new_name);
  TabView& set_active(uint32_t idx, AnimEnable anim = AnimEnable::On);
  TabView& set_tab_bar_position(Dir dir);
  TabView& set_tab_bar_size(int32_t size);

  uint32_t get_tab_count();
  uint32_t get_tab_active();
  lv_obj_t* get_tab_button(int32_t idx);
  lv_obj_t* get_content();
  lv_obj_t* get_tab_bar();
};

}  // namespace lvgl

#endif  // LV_USE_TABVIEW
#endif  // LVGL_CPP_WIDGETS_TABVIEW_H_
