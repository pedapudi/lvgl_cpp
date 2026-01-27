#ifndef LVGL_CPP_CORE_INTERACTION_PROXY_H_
#define LVGL_CPP_CORE_INTERACTION_PROXY_H_

#include "../misc/geometry.h"
#include "lvgl.h"

namespace lvgl {

/**
 * @brief Proxy for interaction-related operations on an object.
 */
class InteractionProxy {
 public:
  explicit InteractionProxy(lv_obj_t* obj);

  /**
   * @brief Increase the clickable area of the object.
   * @param size The amount to increase the area by (in pixels).
   * @return Reference to this proxy for chaining.
   */
  InteractionProxy& set_ext_click_area(int32_t size);

  /**
   * @brief Get the clickable area of the object (including extension).
   * @param area The area to store the result in.
   */
  void get_click_area(Area& area) const;

  /**
   * @brief Check if a point hits the object.
   * @param point The point to check.
   * @return True if the point hits the object, false otherwise.
   */
  bool hit_test(const Point& point) const;

  /**
   * @brief Check if the object is editable (e.g. by encoder).
   * @return True if editable.
   */
  bool is_editable() const;

  /**
   * @brief Check if the object is the default element in its group.
   * @return True if it is the group default.
   */
  bool is_group_def() const;

 private:
  lv_obj_t* obj_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_INTERACTION_PROXY_H_
