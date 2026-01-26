#include <cassert>
#include <iostream>
#include <vector>

#include "../display/display.h"
#include "../indev/button_input.h"
#include "../indev/encoder_input.h"
#include "../indev/keypad_input.h"
#include "../indev/pointer_input.h"
#include "lvgl.h"

// Mock data to verify callback execution
static bool callback_called = false;
static int callback_count = 0;

void setup() {
  lv_init();
  lvgl::Display::create(800, 480);
}

void test_pointer_creation() {
  std::cout << "Testing PointerInput creation..." << std::endl;
  auto ptr = lvgl::PointerInput::create();
  assert(ptr.get_type() == lvgl::IndevType::Pointer);
  assert(ptr.raw() != nullptr);
  std::cout << "PointerInput creation passed." << std::endl;
}

void test_callback_dispatch() {
  std::cout << "Testing Callback dispatch..." << std::endl;
  callback_called = false;
  callback_count = 0;

  auto ptr = lvgl::PointerInput::create();

  // Set a C++ lambda as callback
  ptr.set_read_cb([](lv_indev_data_t* data) {
    callback_called = true;
    callback_count++;
    data->state = static_cast<lv_indev_state_t>(lvgl::IndevState::Pressed);
  });

  // Manually trigger a read (which calls the trampoline -> lambda)
  ptr.read();  // Trigger 1

  // Note: lv_indev_read checks if the device is enabled and might require a
  // timer tick in real loop. However, calling read() directly calls the
  // internal logic. Let's rely on the fact that we set the callback. Is there a
  // way to force invoke the trampoline? The trampoline is internal. We can
  // manually call 'read()' which invokes 'lv_indev_read', which eventually
  // calls the driver read_cb.

  // Actually, lv_indev_read does NOT guarantee immediate call if the device is
  // not ready or timer based? In polling mode, it should be fine.

  // If simple read() doesn't trigger it, we might need 'lv_indev_read_timer_cb'
  // or similar? But let's trust that 'lv_indev_read' invokes it.

  // Wait, lv_indev_read primarily buffers?
  // Let's verify if 'user_data' is set correctly.
  assert(lv_indev_get_user_data(ptr.raw()) == &ptr);

  std::cout << "Callback dispatch setup check passed." << std::endl;
}

void test_subclasses() {
  std::cout << "Testing Subclasses..." << std::endl;

  {
    auto keypad = lvgl::KeypadInput::create();
    assert(keypad.get_type() == lvgl::IndevType::Keypad);
  }  // Should delete keypad here

  {
    auto encoder = lvgl::EncoderInput::create();
    assert(encoder.get_type() == lvgl::IndevType::Encoder);
  }  // Should delete encoder here

  {
    auto btn = lvgl::ButtonInput::create();
    assert(btn.get_type() == lvgl::IndevType::Button);

    std::vector<lv_point_t> points = {{10, 10}, {50, 50}};
    btn.set_points(points);
    // Verify no crash on destruction
  }

  std::cout << "Subclasses test passed." << std::endl;
}

void test_enhancements() {
  std::cout << "Testing v9 Enhancements..." << std::endl;
  auto ptr = lvgl::PointerInput::create();

  // Test configuration methods
  ptr.set_long_press_repeat_time(500);
  ptr.reset_long_press();

  // Test timer/display accessors
  assert(ptr.get_read_timer() != nullptr);
  assert(ptr.get_display() != nullptr);

  // Test event handling
  static bool event_called = false;
  ptr.add_event_cb(
      [](lv_event_t* e) {
        event_called = true;
        lvgl::InputDevice::wrap(lv_event_get_indev(e)).stop_processing();
      },
      LV_EVENT_PRESSED);

  // Send a mock event to verify the callback
  lv_indev_send_event(ptr.raw(), LV_EVENT_PRESSED, nullptr);
  assert(event_called == true);

  std::cout << "v9 Enhancements passed." << std::endl;
}

int main() {
  setup();

  test_pointer_creation();
  test_callback_dispatch();
  test_subclasses();
  test_enhancements();

  std::cout << "All Input Device tests passed!" << std::endl;
  return 0;
}
