#include <cassert>
#include <cstdio>
#include <vector>

#include "../lvgl_cpp.h"
#include "lvgl_cpp/core/object.h"
#include "lvgl_cpp/widgets/arc.h"
#include "lvgl_cpp/widgets/bar.h"
#include "lvgl_cpp/widgets/button.h"
#include "lvgl_cpp/widgets/calendar.h"
#include "lvgl_cpp/widgets/canvas.h"
#include "lvgl_cpp/widgets/chart.h"
#include "lvgl_cpp/widgets/checkbox.h"
#include "lvgl_cpp/widgets/dropdown.h"
#include "lvgl_cpp/widgets/image.h"
#include "lvgl_cpp/widgets/label.h"
#include "lvgl_cpp/widgets/line.h"
#include "lvgl_cpp/widgets/msgbox.h"
#include "lvgl_cpp/widgets/roller.h"
#include "lvgl_cpp/widgets/slider.h"
#include "lvgl_cpp/widgets/switch.h"
#include "lvgl_cpp/widgets/table.h"

// Helper for setup
void setup_lvgl() {
  lv_init();
  // Create a display buffer and display driver if needed for real functional
  // tests But for simple instantiation checks, headless might suffice if LVGL
  // allows. We'll proceed assuming basic minimal setup works.

  // Note: For headless, we usually need at least a display to be registered for
  // screens to work properly. Let's create a minimal dummy display.
  static lv_display_t* disp = lv_display_create(800, 480);
}

int main() {
  setup_lvgl();
  printf("LVGL initialized.\n");

  // Create a base object (Screen)
  lvgl::Object screen;
  assert(screen.is_valid());
  printf("Screen created.\n");

  // Test Button
  {
    lvgl::Button btn(screen);
    assert(btn.is_valid());
    printf("Button created with Object& parent.\n");
  }

  // Test Label
  {
    lvgl::Label lbl(screen);
    assert(lbl.is_valid());
    lvgl::Label lbl_txt(screen, "Hello");
    assert(lbl_txt.is_valid());
    printf("Label created with Object& parent.\n");
  }

  // Test Bar
  {
    lvgl::Bar bar(screen);
    assert(bar.is_valid());
    printf("Bar created with Object& parent.\n");
  }

  // Test Slider
  {
    lvgl::Slider slider(screen);
    assert(slider.is_valid());
    lvgl::Slider slider_range(screen, 0, 100);
    assert(slider_range.is_valid());
    printf("Slider created with Object& parent.\n");
  }

  // Test Switch
  {
    lvgl::Switch sw(screen);
    assert(sw.is_valid());
    printf("Switch created with Object& parent.\n");
  }

  // Test Checkbox
  {
    lvgl::Checkbox cb(screen);
    assert(cb.is_valid());
    lvgl::Checkbox cb_txt(screen, "Check me");
    assert(cb_txt.is_valid());
    printf("Checkbox created with Object& parent.\n");
  }

  // Test Arc
  {
    lvgl::Arc arc(screen);
    assert(arc.is_valid());
    printf("Arc created with Object& parent.\n");
  }

  // Test Line
  {
    lvgl::Line line(screen);
    assert(line.is_valid());
    printf("Line created with Object& parent.\n");
  }

  // Test Dropdown
  {
    lvgl::Dropdown dd(screen);
    assert(dd.is_valid());
    printf("Dropdown created with Object& parent.\n");
  }

  // Test Roller
  {
    lvgl::Roller roller(screen);
    assert(roller.is_valid());
    printf("Roller created with Object& parent.\n");
  }

  // Test Table
  {
    lvgl::Table table(screen);
    assert(table.is_valid());
    printf("Table created with Object& parent.\n");
  }

  // Test Chart
  {
    lvgl::Chart chart(screen);
    assert(chart.is_valid());
    printf("Chart created with Object& parent.\n");
  }

  // Test Calendar
  {
    lvgl::Calendar calendar(screen);
    assert(calendar.is_valid());
    printf("Calendar created with Object& parent.\n");
  }

  // Test MsgBox
  {
    lvgl::MsgBox msgbox(screen);
    assert(msgbox.is_valid());
    printf("MsgBox created with Object& parent.\n");
  }

  // Test Image
  {
    lvgl::Image img(screen);
    assert(img.is_valid());
    printf("Image created with Object& parent.\n");
  }

  // Test Canvas
  {
    lvgl::Canvas canvas(screen);
    assert(canvas.is_valid());
    printf("Canvas created with Object& parent.\n");
  }

  // Test List
  {
    lvgl::List list(screen);
    assert(list.is_valid());
    printf("List created with Object& parent.\n");
  }

  // Test Menu
  {
    lvgl::Menu menu(screen);
    assert(menu.is_valid());
    printf("Menu created with Object& parent.\n");
  }

  // Test Textarea
  {
    lvgl::Textarea ta(screen);
    assert(ta.is_valid());
    lvgl::Textarea ta_txt(screen, "Initial text");
    assert(ta_txt.is_valid());
    printf("Textarea created with Object& parent.\n");
  }

  // Test Spinbox
  {
    lvgl::Spinbox spinbox(screen);
    assert(spinbox.is_valid());
    printf("Spinbox created with Object& parent.\n");
  }

  // Test Spinner
  {
    lvgl::Spinner spinner(screen);
    assert(spinner.is_valid());
    lvgl::Spinner spinner_anim(screen, 1000, 60);
    assert(spinner_anim.is_valid());
    printf("Spinner created with Object& parent.\n");
  }

  // Test TabView
  {
    lvgl::TabView tabview(screen);
    assert(tabview.is_valid());
    printf("TabView created with Object& parent.\n");
  }

  // Test TileView
  {
    lvgl::TileView tileview(screen);
    assert(tileview.is_valid());
    printf("TileView created with Object& parent.\n");
  }

  // Test Win
  {
    lvgl::Win win(screen);
    assert(win.is_valid());
    printf("Win created with Object& parent.\n");
  }

  printf("All passed.\n");
  return 0;
}
