#include <lvgl.h>

#include <iostream>

#include "lvgl_cpp/core/compatibility.h"
#include "lvgl_cpp/display/display.h"
#include "lvgl_cpp/widgets/arc.h"
#include "lvgl_cpp/widgets/bar.h"
#include "lvgl_cpp/widgets/button_matrix.h"
#include "lvgl_cpp/widgets/chart.h"
#include "lvgl_cpp/widgets/checkbox.h"
#include "lvgl_cpp/widgets/keyboard.h"
#include "lvgl_cpp/widgets/led.h"
#include "lvgl_cpp/widgets/line.h"
#include "lvgl_cpp/widgets/menu.h"
#include "lvgl_cpp/widgets/roller.h"
#include "lvgl_cpp/widgets/scale.h"
#include "lvgl_cpp/widgets/slider.h"
#include "lvgl_cpp/widgets/span.h"
#include "lvgl_cpp/widgets/spinbox.h"
#include "lvgl_cpp/widgets/spinner.h"
#include "lvgl_cpp/widgets/switch.h"
#include "lvgl_cpp/widgets/table.h"
#include "lvgl_cpp/widgets/tabview.h"

using namespace lvgl;

static int errors = 0;

#define EXPECT_EQ(actual, expected)                                           \
  do {                                                                        \
    if ((actual) != (expected)) {                                             \
      std::cerr << "FAIL: " << __FILE__ << ":" << __LINE__ << " (" << #actual \
                << " != " << #expected << ") => " << (actual)                 \
                << " != " << (expected) << '\n';                              \
      errors++;                                                               \
    } else {                                                                  \
      std::cout << "PASS: " << #actual << " == " << #expected << '\n';        \
    }                                                                         \
  } while (0)

#if LVGL_CPP_HAS_PROPERTIES
#define EXPECT_PROPERTY_INT(obj, prop, expected)             \
  do {                                                       \
    lv_property_t pv = lv_obj_get_property(obj.raw(), prop); \
    EXPECT_EQ(pv.num, (int32_t)(expected));                  \
  } while (0)
#else
#define EXPECT_PROPERTY_INT(obj, prop, expected) \
  std::cout << "SKIP: Properties not supported in this LVGL version\n"
#endif

void test_arc_properties() {
  std::cout << "Testing Arc properties...\n";
  Arc arc;
  arc.set_value(50);
#if LVGL_CPP_HAS_PROPERTIES
  EXPECT_PROPERTY_INT(arc, LV_PROPERTY_ARC_VALUE, 50);
#endif
}

void test_bar_properties() {
  std::cout << "Testing Bar properties...\n";
  Bar bar;
  bar.set_value(75, AnimEnable::Off);
#if LVGL_CPP_HAS_PROPERTIES
  EXPECT_PROPERTY_INT(bar, LV_PROPERTY_BAR_VALUE, 75);
#endif
}

void test_checkbox_properties() {
  std::cout << "Testing Checkbox properties...\n";
  Checkbox cb;
  cb.set_text("Hello");
#if LVGL_CPP_HAS_PROPERTIES
  lv_property_t pv = lv_obj_get_property(cb.raw(), LV_PROPERTY_CHECKBOX_TEXT);
  EXPECT_EQ(std::string((const char*)pv.ptr), std::string("Hello"));
#endif
}

void test_led_properties() {
  std::cout << "Testing LED properties...\n";
  Led led;
  led.set_brightness(120);
#if LVGL_CPP_HAS_PROPERTIES
  EXPECT_PROPERTY_INT(led, LV_PROPERTY_LED_BRIGHTNESS, 120);
#endif
}

int main() {
  lv_init();
  lvgl::Display display = lvgl::Display::create(800, 480);

  test_arc_properties();
  test_bar_properties();
  test_checkbox_properties();
  test_led_properties();

  if (errors > 0) {
    std::cerr << "Failed with " << errors << " errors.\n";
    return 1;
  }
  std::cout << "All API property tests passed!\n";
  return 0;
}
