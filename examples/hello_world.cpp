#include <unistd.h>

#include <iostream>

#include "../lvgl_cpp.h"

using namespace lvgl;

int main() {
  lv_init();

  // Create a display (required for screen creation)
  // Display display = Display::create(800, 480);

  // Get active screen
  Screen screen = Screen::active();

  // Create a button with fluent method chaining
  Button btn(screen);
  btn.set_size(120, 50)
      .align(Align::Center, 0, 0)
      .style()
      .bg_color(Color::from_hex(0x00AABB))
      .radius(10)
      .shadow_width(20);

  // Add type-safe C++ callback
  btn.add_event_cb(EventCode::Clicked, [](Event e) {
    std::cout << "Lambda clicked! Target: " << e.get_target().raw()
              << std::endl;
  });

  // Create a label
  Label label(btn);
  label.set_text("Hello C++").center();

  // Create a slider
  Slider slider(screen);
  slider.set_size(200, 20)
      .align(Align::BottomMid, 0, -20)
      .set_range(0, 100)
      .set_value(50, AnimEnable::Off);

  std::cout << "Setup complete. Objects created." << std::endl;

  // Chart demo
  Chart chart(screen);
  chart.set_size(100, 100)
      .align(Align::TopRight, -10, 10)
      .style()
      .bg_color(Color::from_hex(0x222222))
      .border_width(0);

  // Calendar
  Calendar cal(screen);
  cal.set_size(200, 200).center().add_flag(ObjFlag::Hidden);

  return 0;
}
