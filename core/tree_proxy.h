#ifndef LVGL_CPP_CORE_TREE_PROXY_H_
#define LVGL_CPP_CORE_TREE_PROXY_H_

#include <cstdint>

#include "lvgl.h"

namespace lvgl {

class Object;

/**
 * @brief Proxy for tree hierarchy operations on an object.
 */
class TreeProxy {
 public:
  explicit TreeProxy(lv_obj_t* obj);

  /**
   * @brief Swap this object with another object in the hierarchy (z-order).
   * @param other The object to swap with.
   * @return Reference to this proxy for chaining.
   */
  TreeProxy& swap(Object& other);

  /**
   * @brief Move this object to a specific child index.
   * @param index The index to move to.
   * @return Reference to this proxy for chaining.
   */
  TreeProxy& move_to_index(int32_t index);

 private:
  lv_obj_t* obj_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_TREE_PROXY_H_
