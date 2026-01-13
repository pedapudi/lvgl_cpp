#include "../lvgl_cpp.h"
#include <iostream>

using namespace lvgl;

void my_event_cb(lv_event_t *e) { std::cout << "Button clicked!" << std::endl; }

int main() {
  // Initialize LVGL (mocking the init call as we might not link against full
  // LVGL here)
  lv_init();

  // Create a screen
  Object screen;

  // Create a button
  Button btn(&screen);
  btn.set_size(100, 50);
  btn.align(LV_ALIGN_CENTER, 0, 0);

  // Add callback
  btn.add_event_cb(LV_EVENT_CLICKED, [](lv_event_t *e) {
    std::cout << "Lambda clicked!" << std::endl;
  });

  // Create a label
  Label label(&btn);
  label.set_text("Hello C++");
  label.align(LV_ALIGN_CENTER, 0, 0);

  // Create a slider
  Slider slider(&screen);
  slider.set_size(200, 20);
  slider.align(LV_ALIGN_BOTTOM_MID, 0, -20);
  slider.set_range(0, 100);
  slider.set_value(50, LV_ANIM_OFF);

  std::cout << "Setup complete." << std::endl;

  // Simulation loop (dummy)
  // while(1) { lv_timer_handler(); usleep(5000); }

  // Chart demo
  Chart chart(&screen);
  chart.set_size(100, 100);
  chart.align(LV_ALIGN_TOP_RIGHT, -10, 10);

  // Calendar
  Calendar cal(&screen);
  cal.set_size(200, 200);
  cal.center();
  cal.add_flag(LV_OBJ_FLAG_HIDDEN); // Hide it for now

  return 0;
}
