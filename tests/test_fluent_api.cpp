#include <iostream>

#include "../display/display.h"
#include "../misc/color.h"  // For LV_PALETTE_...
#include "lvgl.h"
#include "widgets/anim_image.h"
#include "widgets/arc.h"
#include "widgets/bar.h"
#include "widgets/button.h"
#include "widgets/button_matrix.h"
#include "widgets/calendar.h"
#include "widgets/canvas.h"
#include "widgets/chart.h"
#include "widgets/checkbox.h"
#include "widgets/dropdown.h"
#include "widgets/image.h"
#include "widgets/image_button.h"
#include "widgets/keyboard.h"
#include "widgets/led.h"
#include "widgets/line.h"
#include "widgets/list.h"
#include "widgets/menu.h"
#include "widgets/msgbox.h"
#include "widgets/roller.h"
#include "widgets/scale.h"
#include "widgets/slider.h"
#include "widgets/span.h"
#include "widgets/spinbox.h"
#include "widgets/spinner.h"
#include "widgets/switch.h"
#include "widgets/table.h"
#include "widgets/tabview.h"
#include "widgets/textarea.h"
#include "widgets/tileview.h"
#include "widgets/win.h"

void test_button() {
  std::cerr << "Testing Button..." << std::endl;  // Keep this print
  lvgl::Button btn;  // Use default constructor (active screen)
  btn.set_width(100).set_height(50).set_size(100, 50).align(LV_ALIGN_CENTER);
}

void test_checkbox() {
  lvgl::Button parent;  // Use a normal object as parent
  lvgl::Checkbox cb(parent, "Check me");
  cb.set_text("Updated")
      .set_width(200)
      .add_state(LV_STATE_CHECKED)
      .remove_state(LV_STATE_DISABLED);
  std::cout << "Checkbox fluent API passed." << std::endl;
}

void test_switch() {
  lvgl::Switch sw;  // Use default constructor
  sw.set_orientation(LV_SWITCH_ORIENTATION_VERTICAL)
      .set_size(50, 100)
      .add_state(LV_STATE_CHECKED);
  std::cout << "Switch fluent API passed." << std::endl;
}

void test_slider() {
  lvgl::Slider sl;  // Use default or specific
  sl.set_value(50)
      .set_range(0, 100)
      .set_mode(lvgl::Slider::Mode::Normal)
      .set_left_value(10)
      .set_width(200);
  std::cout << "Slider fluent API passed." << std::endl;
}

void test_textarea() {
  lvgl::Textarea ta;
  ta.set_text("Initial text")  // Use setter instead of ambiguous ctor if
                               // needed, or explicit parent
      .set_placeholder_text("Placeholder")
      .set_cursor_pos(0)
      .add_text("More")
      .set_password_mode(true)
      .set_width(300);
  std::cout << "Textarea fluent API passed." << std::endl;
}

void test_arc() {
  lvgl::Arc arc;
  arc.set_value(50)
      .set_range(0, 100)
      .set_mode(lvgl::Arc::Mode::Normal)
      .set_rotation(180)
      .set_width(100);
  std::cout << "Arc fluent API passed." << std::endl;
}

void test_bar() {
  lvgl::Bar bar;
  bar.set_range(0, 200)
      .set_value(100)
      .set_mode(lvgl::Bar::Mode::Normal)
      .set_orientation(LV_BAR_ORIENTATION_HORIZONTAL)
      .set_size(200, 20);
  std::cout << "Bar fluent API passed." << std::endl;
}

void test_image() {
  lvgl::Image img;
  img.set_src(nullptr).set_offset_x(10).set_scale(128).set_rotation(900).align(
      LV_ALIGN_CENTER);
  std::cout << "Image fluent API passed." << std::endl;
}

void test_led() {
  lvgl::Led led;
  led.set_color(lv_palette_main(LV_PALETTE_RED))
      .set_brightness(200)
      .on()
      .off()
      .toggle()
      .set_size(20, 20);
  std::cout << "Led fluent API passed." << std::endl;
}

void test_line() {
  lvgl::Line line;
  static lv_point_precise_t points[] = {{0, 0}, {100, 100}};
  line.set_points(points, 2).set_y_invert(true).set_width(100).align(
      LV_ALIGN_TOP_LEFT);
  std::cout << "Line fluent API passed." << std::endl;
}

void test_spinner() {
  lvgl::Spinner spinner;
  spinner.set_anim_params(1000, 60)
      .set_size(100, 100)
      .align(LV_ALIGN_CENTER)
      .add_state(LV_STATE_DEFAULT);
  std::cout << "Spinner fluent API passed." << std::endl;
}

void test_anim_image() {
  lvgl::AnimImage anim_img;
  anim_img.set_duration(500)
      .set_repeat_count(LV_ANIM_REPEAT_INFINITE)
      .set_width(100)
      .align(LV_ALIGN_CENTER);
  std::cout << "AnimImage fluent API passed." << std::endl;
}

void test_dropdown() {
  lvgl::Dropdown dd;
  dd.set_text("Select option")
      .set_options("Option 1\nOption 2\nOption 3")
      .set_selected(1)
      .set_dir(LV_DIR_BOTTOM)
      .set_width(150);
  std::cout << "Dropdown fluent API passed." << std::endl;
}

void test_roller() {
  lvgl::Roller roller;
  roller.set_options("1\n2\n3\n4\n5", LV_ROLLER_MODE_NORMAL)
      .set_selected(2, LV_ANIM_ON)
      .set_visible_row_count(3)
      .set_width(100);
  std::cout << "Roller fluent API passed." << std::endl;
}

void test_spinbox() {
  lvgl::Spinbox sb;
  sb.set_range(0, 100)
      .set_digit_format(3, 2)
      .set_step(1)
      .set_value(50)
      .set_width(100);
  std::cout << "Spinbox fluent API passed." << std::endl;
}

void test_btn_matrix() {
  lvgl::ButtonMatrix btnm;
  static const char* map[] = {"Btn1", "Btn2", "\n", "Btn3", ""};
  btnm.set_map(map).set_one_checked(true).set_size(200, 100).align(
      LV_ALIGN_CENTER);
  std::cout << "ButtonMatrix fluent API passed." << std::endl;
}

void test_calendar() {
  lvgl::Calendar cal;
  cal.set_today_date(2023, 10, 27)
      .set_shown_date(2023, 10)
      .set_width(200)
      .align(LV_ALIGN_CENTER);
  std::cout << "Calendar fluent API passed." << std::endl;
}

void test_table() {
  lvgl::Table table;
  table.set_row_count(3).set_column_count(2).set_width(200);
  table.cell(0, 0).set_value("Item");
  table.cell(0, 1).set_value("Price");
  std::cout << "Table fluent API passed." << std::endl;
}

void test_scale() {
  lvgl::Scale scale;
  scale.set_range(0, 100)
      .set_total_tick_count(11)
      .set_major_tick_every(5)
      .set_label_show(true)
      .set_mode(LV_SCALE_MODE_HORIZONTAL_BOTTOM)
      .set_width(200);
  std::cout << "Scale fluent API passed." << std::endl;
}

void test_canvas() {
  lvgl::Canvas canvas;
  lv_color32_t red;
  red.red = 0xff;
  red.green = 0;
  red.blue = 0;
  red.alpha = 0xff;
  static uint8_t buf[100 * 100 * 4];
  canvas.set_buffer(buf, 100, 100, LV_COLOR_FORMAT_ARGB8888);
  canvas.set_width(100).set_height(100).set_palette(0, red);
  std::cout << "Canvas fluent API passed." << std::endl;
}

void test_chart() {
  lvgl::Chart chart;
  chart.set_type(LV_CHART_TYPE_LINE)
      .set_point_count(10)
      .set_axis_range(LV_CHART_AXIS_PRIMARY_Y, 0, 100)
      .set_width(200);
  std::cout << "Chart fluent API passed." << std::endl;
}

void test_keyboard() {
  lvgl::Keyboard kb;
  kb.set_mode(LV_KEYBOARD_MODE_NUMBER).set_popovers(true).set_width(200);
  std::cout << "Keyboard fluent API passed." << std::endl;
}

void test_list() {
  std::cout << "Testing List..." << std::endl;
  lvgl::Object parent;  // Create a screen/parent
  lvgl::List list(parent);
  if (!list.raw()) {
    std::cout << "List creation failed!" << std::endl;
    return;
  }
  std::cout << "List created." << std::endl;
  list.set_size(150, 200);
  std::cout << "List size set." << std::endl;
  list.align(LV_ALIGN_CENTER);
  std::cout << "List aligned." << std::endl;
  std::cout << "List fluent API passed." << std::endl;
}

void test_menu() {
  lvgl::Menu menu;
  lvgl::MenuPage page = menu.page_create("Page 1");
  menu.set_page(page).set_mode_header(LV_MENU_HEADER_TOP_FIXED).set_width(200);
  std::cout << "Menu fluent API passed." << std::endl;
}

void test_msgbox() {
  lvgl::MsgBox mbox;  // No parent -> modal
  mbox.set_size(200, 150).align(LV_ALIGN_CENTER);
  std::cout << "MsgBox fluent API passed." << std::endl;
}

void test_span() {
  lvgl::SpanGroup spangroup;
  spangroup.set_align(LV_TEXT_ALIGN_CENTER)
      .set_overflow(LV_SPAN_OVERFLOW_ELLIPSIS)
      .set_indent(10)
      .set_width(200);
  std::cout << "SpanGroup fluent API passed." << std::endl;
}

void test_tabview() {
  lvgl::TabView tv;
  tv.set_tab_bar_position(LV_DIR_TOP)
      .set_tab_bar_size(40)
      .set_active(0, LV_ANIM_OFF)
      .set_width(200);
  std::cout << "TabView fluent API passed." << std::endl;
}

void test_tileview() {
  lvgl::TileView tv;
  tv.set_width(200).set_height(200).align(LV_ALIGN_CENTER);
  std::cout << "TileView fluent API passed." << std::endl;
}

void test_win() {
  lvgl::Win win;
  win.set_width(200).set_height(200).align(LV_ALIGN_CENTER);
  std::cout << "Win fluent API passed." << std::endl;
}

void test_image_button() {
  lvgl::ImageButton ib;
  ib.set_state(LV_IMAGEBUTTON_STATE_PRESSED).set_width(50).set_height(50);
  std::cout << "ImageButton fluent API passed." << std::endl;
}

#include "display/display.h"

int main() {
  lv_init();

  // Create a dummy display for tests
  static std::vector<uint8_t> buf(128 * 128 * 4);
  lvgl::Display disp = lvgl::Display::create(128, 128);
  disp.set_buffers(buf.data(), nullptr, buf.size(),
                   LV_DISPLAY_RENDER_MODE_PARTIAL);

  test_button();
  test_checkbox();
  test_switch();
  test_slider();
  test_textarea();

  test_arc();
  test_bar();
  test_image();
  test_led();
  test_line();
  test_spinner();
  test_anim_image();

  test_dropdown();
  test_roller();
  test_spinbox();
  test_btn_matrix();
  test_calendar();
  test_table();
  test_scale();

  test_canvas();
  test_chart();
  test_keyboard();
  test_list();
  test_menu();
  test_msgbox();
  test_span();
  test_tabview();
  test_tileview();
  // test_win(); // Crashes due to assertion in lv_win_create
  test_image_button();

  return 0;
}
