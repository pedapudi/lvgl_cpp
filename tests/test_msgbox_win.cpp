
#include <cassert>
#include <iostream>

#include "../widgets/button.h"
#include "../widgets/label.h"
#include "../widgets/msgbox.h"
#include "../widgets/win.h"
#include "lvgl.h"

using namespace lvgl;

void test_msgbox_ownership() {
  std::cout << "Testing MsgBox Ownership..." << std::endl;
  Object screen(lv_screen_active(), Object::Ownership::Unmanaged);

  // Test creation and adding elements
  MsgBox mbox(screen);
  mbox.add_title("Alert");
  mbox.add_text("This is a message");

  {
    Button close_btn = mbox.add_close_button();
    // close_btn is unmanaged wrapper.
    // When it goes out of scope, the button should NOT be deleted.
  }

  // Verify consistency via API (not easily possible directly without friend,
  // but crash would happen if close_btn deleted the object)

  // Test synchronous close
  assert(mbox.is_valid());
  mbox.close();
  // After close(), the underlying object is deleted immediately.
  // The wrapper should be marked invalid.
  assert(!mbox.is_valid());

  std::cout << "MsgBox Ownership Passed" << std::endl;
}

void test_win_ownership() {
  std::cout << "Testing Win Ownership..." << std::endl;
  Object screen(lv_screen_active(), Object::Ownership::Unmanaged);

  Win win(screen);
  win.set_size(200, 200);

  {
    Label title = win.add_title("My Window");
    Button btn = win.add_button(LV_SYMBOL_CLOSE, 30);
  }
  // title and btn wrappers destroyed. Objects should persist.

  // Verify content container
  WinContent content = win.get_content();
  assert(content.is_valid());

  // Add child to content
  {
    Label l(content);
    l.set_text("Inside Window");
  }

  std::cout << "Win Ownership Passed" << std::endl;
}

int main() {
  lv_init();
  lv_display_create(800, 480);

  test_msgbox_ownership();
  test_win_ownership();

  return 0;
}
