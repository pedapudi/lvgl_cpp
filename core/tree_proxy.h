#ifndef LVGL_CPP_CORE_TREE_PROXY_H_
#define LVGL_CPP_CORE_TREE_PROXY_H_

#include "object.h"

namespace lvgl {

/**
 * @brief Facet Proxy for object hierarchy management.
 * Provides a scoped API for tree operations (children, parent, clean).
 */
class TreeProxy {
 public:
  explicit TreeProxy(Object* obj) : obj_(obj) {}

  /**
   * @brief Get the parent of the object.
   * @return The parent object.
   */
  Object get_parent() const;

  /**
   * @brief Get the child at a specific index.
   * @param index The index of the child.
   * @return The child object.
   */
  Object get_child(int32_t index) const;

  /**
   * @brief Get the number of children.
   * @return The child count.
   */
  uint32_t get_child_count() const;

  /**
   * @brief Remove all children.
   * @return Reference to the tree proxy.
   */
  TreeProxy& clean();

  /**
   * @brief Get the index of this object among its siblings.
   * @return The index.
   */
  uint32_t get_index() const;

  /**
   * @brief Move this object to the foreground.
   * @return Reference to the tree proxy.
   */
  TreeProxy& move_to_foreground();

  /**
   * @brief Move this object to the background.
   * @return Reference to the tree proxy.
   */
  TreeProxy& move_to_background();

  /**
   * @brief Send the object to a new parent.
   * @param parent The new parent.
   * @return Reference to the tree proxy.
   */
  TreeProxy& set_parent(Object& parent);

 private:
  Object* obj_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_TREE_PROXY_H_
