#ifndef LVGL_CPP_CORE_GROUP_PROXY_H_
#define LVGL_CPP_CORE_GROUP_PROXY_H_

#include "lvgl.h"

namespace lvgl {

class Object;

class Group;

/**
 * @brief Transient proxy object for managing group membership on an object.
 *
 * This class is returned by `Object::group()` and provides a fluent API
 * for managing navigation groups.
 */
class GroupProxy {
 public:
  explicit GroupProxy(Object* obj) : obj_(obj) {}

  /**
   * @brief Add the object to the default group.
   * @return Reference to this proxy for chaining.
   */
  GroupProxy& add();

  /**
   * @brief Add the object to a specific group.
   * @param group The group to add to.
   * @return Reference to this proxy for chaining.
   */
  GroupProxy& add(lv_group_t* group);

  /**
   * @brief Add the object to a specific group.
   * @param group The group to add to.
   * @return Reference to this proxy for chaining.
   */
  GroupProxy& add(Group& group);

  /**
   * @brief Remove the object from its group.
   * @return Reference to this proxy for chaining.
   */
  GroupProxy& remove();

  /**
   * @brief Focus this object within its group.
   * @return Reference to this proxy for chaining.
   */
  GroupProxy& focus();

  /**
   * @brief Check if this object is currently focused.
   * @return true if focused.
   */
  bool is_focused() const;

 private:
  Object* obj_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_GROUP_PROXY_H_
