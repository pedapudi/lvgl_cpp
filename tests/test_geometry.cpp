#include "../misc/geometry.h"

#if LVGL_VERSION_MAJOR == 9

#include <cassert>
#include <iostream>

#include "lv_conf_test.h"

void test_coord() {
  lvgl::Coord c1(10);
  lvgl::Coord c2 = 10;
  assert(c1 == c2);
  assert(c1 == 10);

  lvgl::Coord pct = lvgl::Coord::pct(50);
  assert(pct == LV_PCT(50));
  std::cout << "Coord tests passed." << std::endl;
}

void test_point() {
  lvgl::Point p1(10, 20);
  assert(p1.x() == 10);
  assert(p1.y() == 20);

  lvgl::Point p2(5, 5);
  lvgl::Point p3 = p1 + p2;
  assert(p3.x() == 15);
  assert(p3.y() == 25);

  lvgl::Point p4 = p1 - p2;
  assert(p4.x() == 5);
  assert(p4.y() == 15);

  lv_point_t raw_p = {100, 200};
  lvgl::Point p5(raw_p);
  assert(p5.x() == 100);
  std::cout << "Point tests passed." << std::endl;
}

void test_area() {
  lvgl::Area a1(0, 0, 100, 50);
  assert(a1.get_width() == 101);  // 0..100 is 101 pixels
  assert(a1.get_height() == 51);

  lvgl::Area a2(10, 10, 20, 20);
  a2.move(5, 5);
  // 10+5 = 15, 20+5=25
  lv_area_t* raw = a2.raw();
  assert(raw->x1 == 15);
  assert(raw->y1 == 15);
  assert(raw->x2 == 25);
  assert(raw->y2 == 25);

  a2.set_width(50);
  assert(a2.get_width() == 50);

  std::cout << "Area tests passed." << std::endl;
}

int main() {
  lv_init();
  test_coord();
  test_point();
  test_area();
  return 0;
}

#else

int main() { return 0; }

#endif
