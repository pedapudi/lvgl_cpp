#ifndef LVGL_CPP_MISC_GEOMETRY_H_
#define LVGL_CPP_MISC_GEOMETRY_H_

#include "enums.h"
#include "lvgl.h"

namespace lvgl {

/**
 * @brief Wrapper for lv_coord_t.
 */
class Coord {
 public:
  Coord() : val_(0) {}
  Coord(lv_coord_t val) : val_(val) {}  // NOLINT(google-explicit-constructor)

  operator lv_coord_t() const {
    return val_;
  }  // NOLINT(google-explicit-constructor)

  bool operator==(const Coord& other) const { return val_ == other.val_; }
  bool operator!=(const Coord& other) const { return val_ != other.val_; }

  bool operator==(lv_coord_t other) const { return val_ == other; }
  bool operator!=(lv_coord_t other) const { return val_ != other; }

  static Coord pct(lv_coord_t x) { return Coord(LV_PCT(x)); }

 private:
  lv_coord_t val_;
};

/**
 * @brief Wrapper for lv_point_t.
 */
class Point {
 public:
  Point() : point_{0, 0} {}
  Point(lv_coord_t x, lv_coord_t y) : point_{x, y} {}
  Point(lv_point_t p) : point_(p) {}  // NOLINT(google-explicit-constructor)

  operator lv_point_t() const {
    return point_;
  }  // NOLINT(google-explicit-constructor)

  lv_coord_t x() const { return point_.x; }
  lv_coord_t y() const { return point_.y; }

  void set_x(lv_coord_t x) { point_.x = x; }
  void set_y(lv_coord_t y) { point_.y = y; }

  bool operator==(const Point& other) const {
    return point_.x == other.point_.x && point_.y == other.point_.y;
  }
  bool operator!=(const Point& other) const { return !(*this == other); }

  Point operator+(const Point& other) const {
    return Point(point_.x + other.point_.x, point_.y + other.point_.y);
  }

  Point operator-(const Point& other) const {
    return Point(point_.x - other.point_.x, point_.y - other.point_.y);
  }

  lv_point_t* raw() { return &point_; }
  const lv_point_t* raw() const { return &point_; }

 private:
  lv_point_t point_;
};

/**
 * @brief Wrapper for lv_area_t.
 */
class Area {
 public:
  Area() : area_{0, 0, 0, 0} {}
  Area(lv_coord_t x1, lv_coord_t y1, lv_coord_t x2, lv_coord_t y2)
      : area_{x1, y1, x2, y2} {}
  Area(const Point& p, const Point& s)
      : area_{p.x(), p.y(), p.x() + s.x() - 1, p.y() + s.y() - 1} {}

  operator lv_area_t() const {
    return area_;
  }  // NOLINT(google-explicit-constructor)

  lv_coord_t get_width() const { return lv_area_get_width(&area_); }
  lv_coord_t get_height() const { return lv_area_get_height(&area_); }

  void set_width(lv_coord_t w) { lv_area_set_width(&area_, w); }
  void set_height(lv_coord_t h) { lv_area_set_height(&area_, h); }

  void set_pos(lv_coord_t x, lv_coord_t y) {
    lv_coord_t w = get_width();
    lv_coord_t h = get_height();
    area_.x1 = x;
    area_.y1 = y;
    area_.x2 = x + w - 1;
    area_.y2 = y + h - 1;
  }

  void increase(lv_coord_t w_extra, lv_coord_t h_extra) {
    lv_area_increase(&area_, w_extra, h_extra);
  }

  void move(lv_coord_t x_ofs, lv_coord_t y_ofs) {
    lv_area_move(&area_, x_ofs, y_ofs);
  }

  void align(const Area& to, lvgl::Align align, lv_coord_t ofs_x,
             lv_coord_t ofs_y) {
    lv_area_align(&to.area_, &area_, static_cast<lv_align_t>(align), ofs_x,
                  ofs_y);
  }

  lv_area_t* raw() { return &area_; }
  const lv_area_t* raw() const { return &area_; }

 private:
  lv_area_t area_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_GEOMETRY_H_
