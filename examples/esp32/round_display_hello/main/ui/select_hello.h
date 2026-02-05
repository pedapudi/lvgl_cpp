#pragma once

#include <functional>

#include "lvgl_cpp/core/group.h"
#include "lvgl_cpp/lvgl_cpp.h"

class SelectHello {
 public:
  SelectHello();

  void set_on_backlight_changed(std::function<void(bool)> cb) {
    on_backlight_changed_ = cb;
  }

  /**
   * @brief Initialize and show the selection menu.
   * @param display The display to render on.
   */
  void show_menu(lvgl::Display& display);

  /**
   * @brief Get the input group for the menu.
   *
   * LVGL uses Groups to manage focus and input navigation (e.g. which
   * widget is currently "active" and receiving button presses).
   * @return Pointer to the group.
   */
  lvgl::Group* get_group() { return &group_; }

 private:
  lvgl::Roller roller_;
  lvgl::Button select_btn_;
  lvgl::Label select_btn_label_;
  lvgl::Label hint_label_;

  /**
   * @brief Guard flag to prevent multiple rapid transitions.
   *
   * In event-driven UI, a user might press a button twice very quickly.
   * We use this to ensure we only trigger one screen transition at a time.
   */
  bool in_menu_mode_ = true;

  lvgl::Display* display_ = nullptr;
  lvgl::Group group_;

  /**
   * @brief Stores the user's selection before transitioning.
   *
   * When using lv_async_call, the callback runs later. Since the Roller widget
   * might be deleted by the time the callback runs, we store the necessary
   * data (the selected index) here immediately.
   */
  int pending_selection_ = 0;

  /**
   * @brief Persistent wrapper for the active screen.
   *
   * CRITICAL FOR BEGINNERS: In lvgl_cpp, a wrapper object (like lvgl::Object)
   * stores C++ callback closures. If you create a wrapper as a local variable
   * in a function, it is destroyed when the function returns, and its
   * destructor will UNREGISTER your callbacks from LVGL.
   *
   * By keeping this as a class member, we ensure the wrapper lives as long as
   * the SelectHello instance, keeping our "back" button handler active.
   */
  /**
   * @brief Persistent wrapper for the active screen.
   */
  lvgl::Object active_screen_;
  lvgl::Object menu_screen_;
  lvgl::Object hello_screen_;
  lvgl::Switch bl_switch_;
  lvgl::Timer hint_timer_;
  std::function<void(bool)> on_backlight_changed_;

  /**
   * @brief Load the hello screen for a specific index.
   * @param index The index selected in the roller.
   */
  void load_hello_screen(int index);
};
