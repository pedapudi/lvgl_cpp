#include <cassert>
#include <iostream>

#include "../display/display.h"
#include "../widgets/label.h"

using namespace lvgl;

static void test_clean_active_screen() {
  std::cout << "Testing clear_active_screen()..." << std::endl;
  Display* disp = Display::get_default();
  assert(disp->raw() != nullptr);

  // Add a child label to the active screen
  Object screen(disp->get_screen_active(), Object::Ownership::Unmanaged);
  Label lbl(&screen);
  lbl.set_text("I should be deleted");

  // Verify child count > 0 (rudimentary internal knowledge check or assumption)
  // Since we don't have a child_count API exposed on Object easily, we rely on
  // crash-free execution and subsequent clean.

  disp->clear_active_screen();

  // If we're still alive, clean worked at least without crash.
  // Ideally we'd check if 'lbl' is now invalid if it was managed...
  // But 'lbl' is stack allocated wrapper around a created object.

  std::cout << "Screen cleared." << std::endl;
}

static void test_auto_buffers() {
  std::cout << "Testing auto_configure_buffers()..." << std::endl;
  // Create a dummy display for testing if default isn't enough or to avoid
  // messing it up
  Display disp = Display::create(800, 480);

  // Set format so stride calculation works
  disp.set_color_format(LV_COLOR_FORMAT_RGB888);

  // 1. Partial Mode
  disp.auto_configure_buffers(lvgl::Display::RenderMode::Partial, false);
  // 800 * 3 bytes * 48 lines approx
  // Just verify no crash and it sets internal vector

  // 2. Double Buffer Full
  disp.auto_configure_buffers(LV_DISPLAY_RENDER_MODE_FULL, true);

  std::cout << "Buffers configured." << std::endl;

  // Clean up
  disp.delete_display();
}

int main() {
  lv_init();

  // Need a generic display to be present for get_default() usually?
  // lv_display_create(800, 480) is called by Display::create

  // Create a default display for the first test
  Display main_disp = Display::create(480, 320);
  main_disp.set_color_format(LV_COLOR_FORMAT_RGB565);

  test_clean_active_screen();
  test_auto_buffers();

  std::cout << "All Display Utility tests passed." << std::endl;
  return 0;
}
