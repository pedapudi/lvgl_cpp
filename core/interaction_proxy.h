#ifndef LVGL_CPP_CORE_INTERACTION_PROXY_H_
#define LVGL_CPP_CORE_INTERACTION_PROXY_H_

#include "object.h"

namespace lvgl {

/**
 * @brief Facet Proxy for input and interaction properties.
 * Scopes hit-testing, clickability, and group logic.
 */
class InteractionProxy {
 public:
  explicit InteractionProxy(Object* obj) : obj_(obj) {}

  /**
   * @brief Enable or disable clicking.
   * @param en True to enable, false to disable.
   * @return Reference to the interaction proxy.
   */
  InteractionProxy& set_clickable(bool en);

  /**
   * @brief Check if the object is clickable.
   * @return True if clickable.
   */
  bool is_clickable() const;

  /**
   * @brief Check if a point is over the object (hit test).
   * @param p The point to test.
   * @return True if hit.
   */
  bool hit_test(const Point& p) const;

  /**
   * @brief Check if the object or any of its children are focused.
   * @return True if focused.
   */
  bool is_focused() const;

  /**
   * @brief Add the object to a group.
   * @param group The group.
   * @return Reference to the interaction proxy.
   */
  InteractionProxy& set_group(Group& group);

  /**
   * @brief Add the object to a group.
   * @param group The group (raw pointer).
   * @return Reference to the interaction proxy.
   */
  InteractionProxy& set_group(lv_group_t* group);

  /**
   * @brief Get the associated group.
   * @return The group pointer.
   */
  lv_group_t* get_group() const;

 private:
  Object* obj_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_INTERACTION_PROXY_H_
