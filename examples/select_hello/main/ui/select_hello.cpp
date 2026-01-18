#include "select_hello.h"

#include "hello_world.h"

LV_FONT_DECLARE(lv_font_unscii_8);

SelectHello::SelectHello() {}

void SelectHello::show_menu(lvgl::Display& display) {
  display_ = &display;
  auto screen = display_->get_screen_active();
  lvgl::Object scr(screen);

  // Clean current screen
  lv_obj_clean(scr.raw());

  in_menu_mode_ = true;

  // Create Roller
  roller_ = lvgl::Roller(&scr);
  static const char* options = "Hello World\nHello ESP32\nHello LVGL C++";
  roller_.set_options(options, LV_ROLLER_MODE_INFINITE);

  // Use small 8px font for 128x64 display
  lv_obj_set_style_text_font(roller_.raw(), &lv_font_unscii_8, 0);
  lv_obj_set_style_text_font(roller_.raw(), &lv_font_unscii_8, LV_PART_SELECTED);

  // Ensure background is opaque to prevent artifacts
  lv_obj_set_style_bg_opa(roller_.raw(), LV_OPA_COVER, 0);

  roller_.set_visible_row_count(2);
  roller_.set_width(120);
  roller_.align(LV_ALIGN_TOP_MID, 0, 0);

  // Create Hint Label
  hint_label_ = lvgl::Label(&scr);
  hint_label_.set_text("Next     Select");
  lv_obj_set_style_text_font(hint_label_.raw(), &lv_font_unscii_8, 0);
  hint_label_.align(LV_ALIGN_BOTTOM_MID, 0, -2);
}

void SelectHello::handle_input(bool next, bool enter) {
  if (!display_) return;

  if (in_menu_mode_) {
    if (next) {
      // Cycle to next option
      uint32_t current = roller_.get_selected();
      uint32_t total = roller_.get_option_count();
      roller_.set_selected((current + 1) % total, LV_ANIM_ON);
    } else if (enter) {
      // Select option
      uint32_t selected = roller_.get_selected();
      load_hello_screen(selected);
      in_menu_mode_ = false;
    }
  } else {
    // If not in menu, any button press returns to menu
    if (next || enter) {
      show_menu(*display_);
    }
  }
}

void SelectHello::load_hello_screen(int index) {
  std::string text;
  switch (index) {
    case 0:
      text = "Hello World";
      break;
    case 1:
      text = "Hello ESP32";
      break;
    case 2:
      text = "Hello LVGL C++";
      break;
    default:
      text = "Unknown";
      break;
  }

  // Clean screen and load hello UI
  if (display_) {
    lv_obj_clean(display_->get_screen_active());
    HelloWorld::load(*display_, text);
  }
}
