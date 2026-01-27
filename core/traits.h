#ifndef LVGL_CPP_CORE_TRAITS_H_
#define LVGL_CPP_CORE_TRAITS_H_

#include "lvgl.h"

namespace lvgl {

class Object;

/**
 * @brief Traits for mapping C++ widget types to LVGL C classes.
 * @tparam T The C++ widget type.
 */
template <typename T>
struct class_traits;

/**
 * @brief Specialization for the base Object class.
 */
template <>
struct class_traits<Object> {
  static const lv_obj_class_t* get() { return &lv_obj_class; }
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_TRAITS_H_
