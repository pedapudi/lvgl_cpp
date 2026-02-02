#include <cassert>
#include <iostream>

#include "../widgets/button.h"
#include "../widgets/label.h"
#include "../widgets/tabview.h"
#include "../widgets/tileview.h"
#include "../lvgl_cpp.h"

using namespace lvgl;

void test_tabview_ownership() {
  std::cout << "Testing TabView Ownership..." << std::endl;
  // Wrap the active screen to pass as Object* parent
  Object screen(lv_screen_active(), Object::Ownership::Unmanaged);

  TabView tv(screen);
  tv.set_size(300, 200);

  // Add tabs
  {
    TabPage t1 = tv.add_tab("Tab 1");
    Label l1(t1);
    l1.set_text("Content 1");

    // Verify t1 is unmanaged lifecycle (managed by TabView)
  }

  // t1 out of scope. TabView should still have the tab.
  assert(tv.get_tab_count() == 1);

  TabPage t2 = tv.add_tab("Tab 2");
  assert(tv.get_tab_count() == 2);

  std::cout << "TabView Ownership Passed" << std::endl;
}

void test_tabview_fluent() {
  std::cout << "Testing TabView Fluent API..." << std::endl;
  Object screen(lv_screen_active(), Object::Ownership::Unmanaged);
  TabView tv(screen);

  tv.set_tab_bar_position(lvgl::Dir::Bottom).set_tab_bar_size(40);

  assert(tv.get_tab_count() == 0);

  tv.add_tab("Temp");
  tv.rename_tab(0, "Permanent");

  tv.set_active(0, lvgl::AnimEnable::Off);
  assert(tv.get_tab_active() == 0);

  std::cout << "TabView Fluent API Passed" << std::endl;
}

void test_tileview_ownership() {
  std::cout << "Testing TileView Ownership..." << std::endl;
  Object screen(lv_screen_active(), Object::Ownership::Unmanaged);
  TileView tv(screen);

  {
    Tile t1 = tv.add_tile(0, 0, lvgl::Dir::Bottom);
    Label l1(t1);
    l1.set_text("Tile 0,0");
  }

  // Test API access after scoped object destruction
  tv.set_tile_by_index(0, 0, lvgl::AnimEnable::Off);

  std::cout << "TileView Ownership Passed" << std::endl;
}

void test_tileview_fluent_set() {
  std::cout << "Testing TileView Fluent Set..." << std::endl;
  Object screen(lv_screen_active(), Object::Ownership::Unmanaged);
  TileView tv(screen);

  Tile t1 = tv.add_tile(0, 0, lvgl::Dir::Bottom);
  Tile t2 = tv.add_tile(0, 1, lvgl::Dir::Top);

  // Test the new overload taking reference
  tv.set_tile(t2, lvgl::AnimEnable::Off);

  std::cout << "TileView Fluent Set Passed" << std::endl;
}

int main() {
  lv_init();
  lv_display_create(800, 480);

  test_tabview_ownership();
  test_tabview_fluent();
  test_tileview_ownership();
  test_tileview_fluent_set();

  return 0;
}
