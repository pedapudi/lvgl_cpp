#include <iostream>

#include "../core/object.h"
#include "../display/display.h"
#include "../widgets/button.h"
#include "../widgets/button_matrix.h"
#include "../widgets/label.h"
#include "../widgets/list.h"
#include "../widgets/menu.h"
#include "../widgets/msgbox.h"
#include "../widgets/table.h"
#include "../widgets/tabview.h"
#include "../widgets/tileview.h"
#include "../widgets/win.h"
#include "lvgl.h"

using namespace lvgl;

// Helper to get active screen wrapper
Object get_screen() { return Object(lv_screen_active()); }

void test_list_1() {
  std::cout << "Testing List Example 1..." << std::endl;

  if (!lv_screen_active()) {
    std::cout << "Creating default display..." << std::endl;
    lv_display_create(800, 480);
  }

  std::cout << "Active screen: " << lv_screen_active() << std::endl;

  Object screen = get_screen();
  List list1(screen);
  list1.set_size(180, 220);
  list1.center();

  list1.add_text("File");
  Button btn = list1.add_button(LV_SYMBOL_FILE, "New");
  list1.add_button(LV_SYMBOL_DIRECTORY, "Open");
  list1.add_button(LV_SYMBOL_SAVE, "Save");
  list1.add_text("Connectivity");
  list1.add_button(LV_SYMBOL_BLUETOOTH, "Bluetooth");

  std::cout << "List Example 1 Passed" << std::endl;
}

void test_table_1() {
  std::cout << "Testing Table Example 1..." << std::endl;
  Object screen = get_screen();
  Table table(screen);

  // Fill first column
  table.set_cell_value(0, 0, "Name");
  table.set_cell_value(1, 0, "Apple");
  table.set_cell_value(2, 0, "Banana");
  table.set_cell_value(3, 0, "Lemon");

  // Fill second column
  table.set_cell_value(0, 1, "Price");
  table.set_cell_value(1, 1, "$7");
  table.set_cell_value(2, 1, "$4");
  table.set_cell_value(3, 1, "$6");

  table.set_height(200);
  table.center();

  std::cout << "Table Example 1 Passed" << std::endl;
}

void test_tabview_1() {
  std::cout << "Testing Tabview Example 1..." << std::endl;
  Object screen = get_screen();
  TabView tabview(screen);

  TabPage tab1 = tabview.add_tab("Tab 1");
  Label label1(tab1);
  label1.set_text("This is the first tab\n\nContent...");

  TabPage tab2 = tabview.add_tab("Tab 2");
  Label label2(tab2);
  label2.set_text("Second tab");

  TabPage tab3 = tabview.add_tab("Tab 3");
  Label label3(tab3);
  label3.set_text("Third tab");

  std::cout << "Tabview Example 1 Passed" << std::endl;
}

void test_tileview_1() {
  std::cout << "Testing Tileview Example 1..." << std::endl;
  Object screen = get_screen();
  TileView tv(screen);

  // Tile1: 0,0
  // Tile1: 0,0
  Tile tile1 = tv.add_tile(0, 0, LV_DIR_BOTTOM);
  Label label1(tile1);
  label1.set_text("Scroll down");
  label1.center();

  // Tile2: 0,1
  Tile tile2 = tv.add_tile(0, 1, (lv_dir_t)(LV_DIR_TOP | LV_DIR_RIGHT));
  Button btn(tile2);
  Label btn_label(btn);
  btn_label.set_text("Scroll up or right");
  btn.set_size(LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  btn.center();

  // Tile3: 1,1
  Tile tile3 = tv.add_tile(1, 1, LV_DIR_LEFT);
  List list(tile3);
  list.set_size(LV_PCT(100), LV_PCT(100));
  list.add_button(NULL, "One");
  list.add_button(NULL, "Two");

  std::cout << "Tileview Example 1 Passed" << std::endl;
}

void test_win_1() {
  std::cout << "Testing Win Example 1..." << std::endl;
  Object screen = get_screen();
  Win win(screen);
  win.add_title("A title");
  win.add_button(LV_SYMBOL_CLOSE, 60);

  WinContent cont = win.get_content();
  Label label(cont);
  label.set_text("This is a window");

  std::cout << "Win Example 1 Passed" << std::endl;
}

void test_msgbox_1() {
  std::cout << "Testing MsgBox Example 1..." << std::endl;
  // MsgBox parent NULL -> Modal
  MsgBox mbox;
  mbox.add_title("Hello");
  mbox.add_text("This is a message box");
  mbox.add_close_button();
  mbox.add_footer_button("Apply");
  mbox.add_footer_button("Cancel");

  std::cout << "MsgBox Example 1 Passed" << std::endl;
}

void test_menu_1() {
  std::cout << "Testing Menu Example 1..." << std::endl;
  Object screen = get_screen();
  Menu menu(screen);
  menu.set_size(320, 240);
  menu.center();

  MenuPage main_page = menu.page_create(NULL);
  menu.set_page(main_page);

  MenuCont cont = menu.cont_create(main_page);

  Label label(cont);
  label.set_text("Item 1");

  std::cout << "Menu Example 1 Passed" << std::endl;
}

void test_btnmatrix_1() {
  std::cout << "Testing ButtonMatrix Example 1..." << std::endl;
  Object screen = get_screen();
  ButtonMatrix btnm(screen);

  static const char* btnm_map[] = {"1", "2", "3", "\n", "4", "5", "6", ""};
  btnm.set_map(btnm_map);
  btnm.align(LV_ALIGN_CENTER, 0, 0);

  std::cout << "ButtonMatrix Example 1 Passed" << std::endl;
}

int main() {
  lv_init();
  lvgl::Display display = lvgl::Display::create(800, 480);

  test_list_1();
  test_table_1();
  test_tabview_1();
  test_tileview_1();
  test_win_1();
  test_msgbox_1();
  test_menu_1();
  test_btnmatrix_1();

  std::cout << "All Complex Widget Tests Passed!" << std::endl;
  return 0;
}
