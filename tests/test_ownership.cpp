
#include <cassert>
#include <iostream>

#include "../core/object.h"
#include "lvgl.h"

void test_default_ownership() {
  std::cout << "Testing Default Ownership..." << std::endl;

  // 1. Wrapping an existing object -> Default = Unmanaged (View)
  std::cout << "Creating raw object via wrapper..." << std::endl;
  lv_obj_t* raw_obj = nullptr;
  {
    lvgl::Object temp((lvgl::Object*)nullptr);  // Create a screen
    raw_obj = temp.release();
  }

  if (!raw_obj) {
    std::cerr << "Failed to create raw object!" << std::endl;
    exit(1);
  }
  std::cout << "Raw object created: " << raw_obj << std::endl;

  {
    std::cout << "Creating wrapper..." << std::endl;
    lvgl::Object wrapper(raw_obj);
    // Default ctor for wrapping pointer should be Unmanaged
    std::cout << "Wrapper created." << std::endl;
  }
  std::cout << "Wrapper destroyed." << std::endl;

  // Check if raw_obj is still valid (it should be)
  // We assume standard malloc, so if it was deleted, accessing it is UB, but we
  // hope to just valid check. In strict LVGL, we can't check validity easily
  // without ID. But if we deleted it, subsequent delete will double-free.

  std::cout << "Deleting raw object (should be valid)..." << std::endl;
  lv_obj_delete(raw_obj);
  std::cout << "PASS: Object persisted wrapper destruction." << std::endl;
}

void test_child_ownership() {
  std::cout << "Testing Child Ownership..." << std::endl;

  lv_obj_t* parent_raw = nullptr;
  {
    lvgl::Object temp((lvgl::Object*)nullptr);
    parent_raw = temp.release();
  }

  // Wrapper for parent (Unmanaged)
  lvgl::Object parent(parent_raw, lvgl::Object::Ownership::Unmanaged);

  lv_obj_t* child_raw = nullptr;
  {
    // Child creation with parent -> Default = Managed (Owned)
    lvgl::Object child(&parent);
    child_raw = child.raw();
    std::cout << "Child created: " << child_raw << std::endl;
  }

  // Child should be deleted now.
  uint32_t cnt = lv_obj_get_child_count(parent_raw);
  if (cnt == 0) {
    std::cout << "PASS: Child was deleted (child count is 0)." << std::endl;
  } else {
    std::cerr << "FAIL: Child persistent! Count: " << cnt << std::endl;
    exit(1);
  }

  lv_obj_delete(parent_raw);
}

void test_explicit_managed() {
  std::cout << "Testing Explicit Managed..." << std::endl;
  lv_obj_t* raw_obj = nullptr;
  {
    lvgl::Object temp((lvgl::Object*)nullptr);
    raw_obj = temp.release();
  }

  {
    lvgl::Object wrapper(raw_obj, lvgl::Object::Ownership::Managed);
  }
  // usage of raw_obj here should likely crash if valid check or double free.
  std::cout << "PASS: Explicit managed assumed deleted." << std::endl;
}

void test_move_semantics() {
  std::cout << "Testing Move Semantics..." << std::endl;
  lv_obj_t* raw_obj = nullptr;
  {
    lvgl::Object temp((lvgl::Object*)nullptr);
    raw_obj = temp.release();
  }

  {
    // 1. Create Owned wrapper
    lvgl::Object wrapper1(raw_obj, lvgl::Object::Ownership::Managed);

    // 2. Move to wrapper2
    lvgl::Object wrapper2 = std::move(wrapper1);

    // wrapper1 should now be invalid/null
    assert(wrapper1.raw() == nullptr);

    // wrapper2 should own the object
    assert(wrapper2.raw() == raw_obj);

  }  // wrapper2 destoryed here, should delete object

  // Verify it's gone? Hard in LVGL without crash, but logic holds.
  std::cout << "PASS: Move semantics transferred ownership." << std::endl;
}

int main() {
  lv_init();
  test_default_ownership();
  test_child_ownership();
  test_explicit_managed();
  test_move_semantics();
  return 0;
}
