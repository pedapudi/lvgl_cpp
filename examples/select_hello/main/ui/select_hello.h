#pragma once

#include "lvgl_cpp/lvgl_cpp.h"

class SelectHello {
 public:
  SelectHello();

  /**
   * @brief Initialize and show the selection menu.
   * @param display The display to render on.
   */
  void show_menu(lvgl::Display& display);

  /**
   * @brief Handle button input.
   * @param next True if "Next" button was pressed.
   * @param enter True if "Enter" button was pressed.
   */
  void handle_input(bool next, bool enter);

 private:
  lvgl::Roller roller_;
  lvgl::Label hint_label_;
  bool in_menu_mode_ = true;
  lvgl::Display* display_ = nullptr;

  void load_hello_screen(int index);
};
