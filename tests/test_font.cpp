#include <cassert>
#include <iostream>

#include "../font/owned_font.h"
#include "../lvgl_cpp.h"

int main() {
  lv_init();

  std::cout << "Testing OwnedFont..." << std::endl;

  // Test Move Semantics (with invalid font essentially)
  {
    lvgl::OwnedFont f1;
    assert(!f1.is_valid());

    lvgl::OwnedFont f2 = std::move(f1);
    assert(!f2.is_valid());
  }

  // Test Load (Will fail but verify API linkage)
  {
    // Try loading a non-existent file. Should return invalid font (or null
    // inner) Check lvgl log for error if enabled
    lvgl::OwnedFont f = lvgl::OwnedFont::load_bin("non_existent_font.bin");
    // lv_binfont_create returns NULL if failed
    assert(!f.is_valid());
    if (!f.is_valid()) {
      std::cout << "Correctly handled missing font file." << std::endl;
    }
  }

  std::cout << "[SUCCESS] OwnedFont basic lifecycle verified." << std::endl;
  return 0;
}
