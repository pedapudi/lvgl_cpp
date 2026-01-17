
#include <cassert>
#include <iostream>

#include "core/object.h"
#include "lvgl.h"

using namespace lvgl;

void test_align_enum_values() {
  assert(static_cast<int>(Object::Align::Default) == LV_ALIGN_DEFAULT);
  assert(static_cast<int>(Object::Align::TopLeft) == LV_ALIGN_TOP_LEFT);
  assert(static_cast<int>(Object::Align::TopMid) == LV_ALIGN_TOP_MID);
  assert(static_cast<int>(Object::Align::TopRight) == LV_ALIGN_TOP_RIGHT);
  assert(static_cast<int>(Object::Align::BottomLeft) == LV_ALIGN_BOTTOM_LEFT);
  assert(static_cast<int>(Object::Align::BottomMid) == LV_ALIGN_BOTTOM_MID);
  assert(static_cast<int>(Object::Align::BottomRight) == LV_ALIGN_BOTTOM_RIGHT);
  assert(static_cast<int>(Object::Align::LeftMid) == LV_ALIGN_LEFT_MID);
  assert(static_cast<int>(Object::Align::RightMid) == LV_ALIGN_RIGHT_MID);
  assert(static_cast<int>(Object::Align::Center) == LV_ALIGN_CENTER);

  // Outer alignments
  assert(static_cast<int>(Object::Align::OutTopLeft) == LV_ALIGN_OUT_TOP_LEFT);
  assert(static_cast<int>(Object::Align::OutTopMid) == LV_ALIGN_OUT_TOP_MID);
  assert(static_cast<int>(Object::Align::OutTopRight) ==
         LV_ALIGN_OUT_TOP_RIGHT);
  assert(static_cast<int>(Object::Align::OutBottomLeft) ==
         LV_ALIGN_OUT_BOTTOM_LEFT);
  assert(static_cast<int>(Object::Align::OutBottomMid) ==
         LV_ALIGN_OUT_BOTTOM_MID);
  assert(static_cast<int>(Object::Align::OutBottomRight) ==
         LV_ALIGN_OUT_BOTTOM_RIGHT);
  assert(static_cast<int>(Object::Align::OutLeftTop) == LV_ALIGN_OUT_LEFT_TOP);
  assert(static_cast<int>(Object::Align::OutLeftMid) == LV_ALIGN_OUT_LEFT_MID);
  assert(static_cast<int>(Object::Align::OutLeftBottom) ==
         LV_ALIGN_OUT_LEFT_BOTTOM);
  assert(static_cast<int>(Object::Align::OutRightTop) ==
         LV_ALIGN_OUT_RIGHT_TOP);
  assert(static_cast<int>(Object::Align::OutRightMid) ==
         LV_ALIGN_OUT_RIGHT_MID);
  assert(static_cast<int>(Object::Align::OutRightBottom) ==
         LV_ALIGN_OUT_RIGHT_BOTTOM);

  std::cout << "Align enum values verified." << std::endl;
}

void test_align_function() {
  lv_init();
  Object screen;  // Default constructor = Screen

  Object obj(&screen, Object::Ownership::Managed);
  obj.set_size(100, 100);

  obj.align(Object::Align::Center);

  Object obj2(&screen, Object::Ownership::Managed);
  obj2.align_to(obj, Object::Align::OutBottomMid, 0, 10);

  std::cout << "Align functions executed." << std::endl;
}

int main() {
  test_align_enum_values();
  // test_align_function(); // Requires display setup, skipping for API
  // verification
  return 0;
}
