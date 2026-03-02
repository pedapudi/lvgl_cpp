#ifndef LVGL_CPP_MISC_MATH_3D_H_
#define LVGL_CPP_MISC_MATH_3D_H_

#include "lvgl.h"

#if LV_USE_GLTF

namespace lvgl {

/**
 * @brief Wrapper for lv_3dpoint_t.
 */
struct Point3D : public lv_3dpoint_t {
  Point3D() : Point3D(0, 0, 0) {}
  Point3D(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }
  Point3D(const lv_3dpoint_t& p) : lv_3dpoint_t(p) {}
};

/**
 * @brief Wrapper for lv_quaternion_t.
 */
struct Quaternion : public lv_quaternion_t {
  Quaternion() : Quaternion(0, 0, 0, 1) {}
  Quaternion(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }
  Quaternion(const lv_quaternion_t& q) : lv_quaternion_t(q) {}
};

/**
 * @brief Wrapper for lv_3dplane_t.
 */
struct Plane3D : public lv_3dplane_t {
  Plane3D() : Plane3D({0, 0, 0}, {0, 1, 0}) {}
  Plane3D(const Point3D& origin, const Point3D& direction) {
    this->origin = origin;
    this->direction = direction;
  }
  Plane3D(const lv_3dplane_t& p) : lv_3dplane_t(p) {}

  static Plane3D ground(float elevation = 0.0f) {
    return Plane3D(lv_get_ground_plane(elevation));
  }
};

/**
 * @brief Wrapper for lv_3dray_t.
 */
using Ray3D = Plane3D;

}  // namespace lvgl

#endif  // LV_USE_GLTF

#endif  // LVGL_CPP_MISC_MATH_3D_H_
