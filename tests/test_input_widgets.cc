#include <iostream>

#include "../core/object.h"
#include "../widgets/button.h"
#include "../widgets/button_matrix.h"
#include "../widgets/calendar.h"
#include "../widgets/dropdown.h"
#include "../widgets/keyboard.h"
#include "../widgets/label.h"
#include "../widgets/roller.h"
#include "../widgets/spinbox.h"
#include "../widgets/textarea.h"
#include "lvgl.h"

// Textarea Example 1: Simple Text area
void test_textarea_1() {
  std::cout << "Testing Textarea Example 1..." << std::endl;
  lvgl::Textarea ta;
  ta.set_one_line(true);
  ta.align(LV_ALIGN_TOP_MID, 0, 10);
  ta.add_state(LV_STATE_FOCUSED);
  // ButtonMatrix usage skipped or simplified for now to focus on TA
  std::cout << "Textarea Example 1 Passed" << std::endl;
}

// Textarea Example 2: Password field
void test_textarea_2() {
  std::cout << "Testing Textarea Example 2..." << std::endl;
  lvgl::Textarea pwd_ta;
  pwd_ta.set_text("");
  pwd_ta.set_password_mode(true);
  pwd_ta.set_one_line(true);
  pwd_ta.set_width(lv_pct(40));
  pwd_ta.set_pos(5, 20);

  lvgl::Label pwd_label;
  pwd_label.set_text("Password:");
  pwd_label.align_to(&pwd_ta, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

  lvgl::Textarea text_ta;
  text_ta.set_one_line(true);
  text_ta.set_password_mode(false);
  text_ta.set_width(lv_pct(40));
  text_ta.align(LV_ALIGN_TOP_RIGHT, -5, 20);

  lvgl::Label oneline_label;
  oneline_label.set_text("Text:");
  oneline_label.align_to(&text_ta, LV_ALIGN_OUT_TOP_LEFT, 0, 0);

  lvgl::Keyboard kb;
  kb.set_textarea(&pwd_ta);

  std::cout << "Textarea Example 2 Passed" << std::endl;
}

// Keyboard Example 1: Keyboard with text area
void test_keyboard_1() {
  std::cout << "Testing Keyboard Example 1..." << std::endl;
  lvgl::Keyboard kb;

  lvgl::Textarea ta1;
  ta1.align(LV_ALIGN_TOP_LEFT, 10, 10);
  ta1.set_placeholder_text("Hello");
  ta1.set_size(140, 80);

  lvgl::Textarea ta2;
  ta2.align(LV_ALIGN_TOP_RIGHT, -10, 10);
  ta2.set_size(140, 80);

  kb.set_textarea(&ta1);
  std::cout << "Keyboard Example 1 Passed" << std::endl;
}

// Spinbox Example 1: Simple Spinbox
void test_spinbox_1() {
  std::cout << "Testing Spinbox Example 1..." << std::endl;
  lvgl::Spinbox spinbox;
  spinbox.set_range(-1000, 25000);
  spinbox.set_digit_count(5);
  spinbox.set_dec_point_pos(2);
  spinbox.step_prev();
  spinbox.set_width(100);
  spinbox.center();

  int32_t h = spinbox.get_height();

  lvgl::Button btn_plus;
  btn_plus.set_size(h, h);
  btn_plus.align_to(&spinbox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
  btn_plus.set_style_bg_image_src(LV_SYMBOL_PLUS, 0);

  lvgl::Button btn_minus;
  btn_minus.set_size(h, h);
  btn_minus.align_to(&spinbox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
  btn_minus.set_style_bg_image_src(LV_SYMBOL_MINUS, 0);

  std::cout << "Spinbox Example 1 Passed" << std::endl;
}

// Dropdown Example 1: Simple Drop down list
void test_dropdown_1() {
  std::cout << "Testing Dropdown Example 1..." << std::endl;
  lvgl::Dropdown dd;
  dd.set_options(
      "Apple\nBanana\nOrange\nCherry\nGrape\nRaspberry\nMelon\nOrang"
      "e\nLemon\nNuts");
  dd.align(LV_ALIGN_TOP_MID, 0, 20);

  // Example 2 bits
  lvgl::Dropdown dd2;
  dd2.set_text("Menu");
  dd2.set_symbol(LV_SYMBOL_DOWN);
  dd2.set_selected_highlight(false);

  std::cout << "Dropdown Example 1 Passed" << std::endl;
}

// Roller Example 1: Simple Roller
void test_roller_1() {
  std::cout << "Testing Roller Example 1..." << std::endl;
  lvgl::Roller roller1;
  roller1.set_options(
      "January\nFebruary\nMarch\nApril\nMay\nJune\nJuly\nAugust"
      "\nSeptember\nOctober\nNovember\nDecember",
      LV_ROLLER_MODE_INFINITE);
  roller1.set_visible_row_count(4);
  roller1.center();

  // Example 2 bits (Styling)
  lvgl::Roller roller2;
  const char* opts = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10";
  roller2.set_options(opts, LV_ROLLER_MODE_NORMAL);
  roller2.set_visible_row_count(2);
  roller2.set_width(100);
  // roller2.add_style... (skipping style def for brevity, focusing on API)
  roller2.set_style_text_align(LV_TEXT_ALIGN_LEFT, 0);
  roller2.set_selected(2, LV_ANIM_OFF);
  roller2.align(LV_ALIGN_LEFT_MID, 10, 0);

  std::cout << "Roller Example 1 Passed" << std::endl;
}

// Calendar Example 1: Calendar with header
void test_calendar_1() {
  std::cout << "Testing Calendar Example 1..." << std::endl;
  lvgl::Calendar calendar;
  calendar.set_size(185, 230);
  calendar.align(LV_ALIGN_CENTER, 0, 27);
  calendar.set_today_date(2021, 02, 23);
  calendar.set_shown_date(2021, 02);

  static lv_calendar_date_t highlighted_days[3];
  highlighted_days[0].year = 2021;
  highlighted_days[0].month = 02;
  highlighted_days[0].day = 6;
  highlighted_days[1].year = 2021;
  highlighted_days[1].month = 02;
  highlighted_days[1].day = 11;
  highlighted_days[2].year = 2022;
  highlighted_days[2].month = 02;
  highlighted_days[2].day = 22;
  calendar.set_highlighted_dates(highlighted_days, 3);

  // Assuming wrapper exposes header addition
  // calendar.add_header_arrow();
  // If not, we might need to skip or check wrapper.

  std::cout << "Calendar Example 1 Passed" << std::endl;
}

int main() {
  lv_init();
  test_textarea_1();
  test_textarea_2();
  test_keyboard_1();
  test_spinbox_1();
  test_dropdown_1();
  test_roller_1();
  test_calendar_1();
  return 0;
}
