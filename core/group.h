#ifndef LVGL_CPP_CORE_GROUP_H_
#define LVGL_CPP_CORE_GROUP_H_

#include <cstdint>

#include "lvgl.h"    // IWYU pragma: export
#include "object.h"  // IWYU pragma: export

namespace lvgl {

/**
 * @brief Wrapper for lv_group_t.
 * Groups are used to control multiple objects with an encoder or keyboard.
 */
class Group {
 public:
  Group();
  ~Group();

  /**
   * @brief Add an object to the group.
   * @param obj The object to add.
   */
  void add_obj(Object* obj);

  /**
   * @brief Remove an object from the group.
   * @param obj The object to remove.
   */
  void remove_obj(Object* obj);

  /**
   * @brief Remove all objects from the group.
   */
  void remove_all_objs();

  /**
   * @brief Focus on a specific object.
   * @param obj The object to focus.
   */
  void focus_obj(Object* obj);

  /**
   * @brief Focus the next object in the group.
   */
  void focus_next();

  /**
   * @brief Focus the previous object in the group.
   */
  void focus_prev();

  /**
   * @brief Freeze the focus (prevent changing focus).
   * @param en true to freeze, false to unfreeze.
   */
  void focus_freeze(bool en);

  /**
   * @brief Set this group as the default group for new objects.
   */
  void set_default();

  /**
   * @brief Get the default group.
   * @return Pointer to the default Group wrapper.
   * @note The returned pointer represents the global default group.
   *       Ownership is not transferred.
   */
  static Group* get_default();

  /**
   * @brief Set editing mode for the group.
   * @param edit true to enable editing mode, false for navigation mode.
   */
  void set_editing(bool edit);

  /**
   * @brief Enable or disable wrapping (cyclic focus).
   * @param en true to enable wrapping.
   */
  void set_wrap(bool en);

  /**
   * @brief Get the currently focused object.
   * @return Pointer to the focused object, or nullptr.
   */
  lv_obj_t* get_focused();

  /**
   * @brief Check if the group is in editing mode.
   * @return true if editing, false if navigating.
   */
  bool get_editing();

  /**
   * @brief Check if wrapping is enabled.
   * @return true if wrapping is enabled.
   */
  bool get_wrap();

  /**
   * @brief Get the number of objects in the group.
   * @return The object count.
   */
  uint32_t get_obj_count();

  lv_group_t* raw() const { return group_; }

 private:
  lv_group_t* group_;
  bool owned_;

  // Private constructor for wrapping existing group without taking ownership
  // (or maybe default group shouldn't be deleted)
  explicit Group(lv_group_t* g, bool owned = false);
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_GROUP_H_
