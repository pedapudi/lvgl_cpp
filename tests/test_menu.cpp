#include <iostream>

#include "../core/object.h"
#include "../display/display.h"
#include "../widgets/button.h"
#include "../widgets/label.h"
#include "../widgets/menu.h"
#include "lvgl.h"

void test_menu_basic() {
  std::cout << "Testing Menu Basic..." << std::endl;
  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::Menu menu(screen);

  // Verify creation
  if (!menu.is_valid()) {
    std::cerr << "FAIL: Menu creation failed." << std::endl;
    exit(1);
  }

  // Create Pages
  lvgl::MenuPage main_page = menu.page_create("Main Page");
  lvgl::MenuPage sub_page = menu.page_create("Sub Page");

  // Verify factory returns valid objects
  if (!main_page.is_valid() || !sub_page.is_valid()) {
    std::cerr << "FAIL: Page creation failed." << std::endl;
    exit(1);
  }

  // Verify ownership is unmanaged (default for page_create in our design)
  // Actually, we need to verify that when they go out of scope, they don't
  // destroy the page if the menu still holds them. But wait, `page_create`
  // returns by value. The C++ wrapper destructor will be called. If it's
  // MANAGED, it will delete the page. Since `lv_menu` manages pages, the
  // wrapper MUST be Unmanaged. We can't easily check internal flag via public
  // API, but if we assign strict scope, we can check validity.
}

void test_menu_navigation() {
  std::cout << "Testing Menu Navigation..." << std::endl;
  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::Menu menu(screen);
  menu.set_size(320, 240);  // Ensure size for visibility logic if any

  lvgl::MenuPage main_page = menu.page_create("Main");
  lvgl::MenuCont cont = menu.cont_create(main_page);
  lvgl::Label label(cont);
  label.set_text("Hello Menu");

  lvgl::MenuPage sub_page = menu.page_create("Settings");

  // Set main page
  menu.set_page(main_page);
  menu.set_sidebar_page(sub_page);

  // Check current page
  lvgl::MenuPage cur = menu.get_cur_main_page();
  if (cur.raw() == main_page.raw()) {
    std::cout << "PASS: Current main page matches." << std::endl;
  } else {
    std::cerr << "FAIL: Current main page mismatch." << std::endl;
    exit(1);
  }
}

void test_load_page_event() {
  std::cout << "Testing Load Page Event..." << std::endl;
  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::Menu menu(screen);

  lvgl::MenuPage main_page = menu.page_create("Main");
  lvgl::MenuCont cont = menu.cont_create(main_page);

  lvgl::Button btn(cont);  // Child of page
  lvgl::MenuPage dest_page = menu.page_create("Destination");

  // This should compile and work
  menu.set_load_page_event(btn, dest_page);

  std::cout << "PASS: set_load_page_event compiled and ran." << std::endl;
}

int main() {
  lv_init();
  lvgl::Display display = lvgl::Display::create(800, 480);

  test_menu_basic();
  test_menu_navigation();
  test_load_page_event();

  return 0;
}
