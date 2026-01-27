#ifndef LVGL_CPP_CORE_CAST_H_
#define LVGL_CPP_CORE_CAST_H_

#include "lvgl.h"
#include "object.h"
#include "traits.h"

namespace lvgl {

/**
 * @brief Check if an object is of a specific type (including ancestors).
 * @tparam T The C++ widget type to check against (e.g., Button).
 * @param obj The object to inspect.
 * @return true if the object belongs to the specified class or any of its
 * subclasses.
 */
template <typename T>
bool is(const Object& obj) {
  if (!obj.is_valid()) return false;
  return lv_obj_has_class(obj.raw(), class_traits<T>::get());
}

/**
 * @brief Check if an object is of a specific type (exact match).
 * @tparam T The C++ widget type to check against.
 * @param obj The object to inspect.
 * @return true if the object belongs exactly to the specified class.
 */
template <typename T>
bool is_exact(const Object& obj) {
  if (!obj.is_valid()) return false;
  return lv_obj_check_type(obj.raw(), class_traits<T>::get());
}

/**
 * @brief Safely cast an Object to a derived Widget type.
 * @tparam T The target C++ widget type.
 * @param obj The object to cast.
 * @return A new instance of T wrapping the same raw object if the cast is
 * valid, otherwise an invalid (null) instance of T.
 * @note The returned object is always unmanaged (it does not take ownership).
 */
template <typename T>
T cast(const Object& obj) {
  if (is<T>(obj)) {
    return T(obj.raw(), Object::Ownership::Unmanaged);
  }
  return T(static_cast<lv_obj_t*>(nullptr));
}

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_CAST_H_
