#pragma once

#include <memory>

#include "lvgl.h"  // IWYU pragma: export
#include "object.h"

namespace lvgl {

/**
 * @brief Wrapper for lv_group_t, managing input device focus.
 */
class Group {
 public:
  /**
   * @brief Ownership policy for the underlying lv_group_t.
   */
  enum class Ownership {
    Managed,   ///< The C++ object owns the lv_group_t and deletes it on
               ///< destruction.
    Unmanaged  ///< The C++ object does not own the lv_group_t.
  };

  /**
   * @brief Create a new Group.
   * @param ownership Ownership policy (default: Managed).
   */
  explicit Group(Ownership ownership = Ownership::Managed);

  /**
   * @brief Wrap an existing lv_group_t.
   * @param group The raw LVGL group.
   * @param ownership Ownership policy.
   */
  explicit Group(lv_group_t* group, Ownership ownership = Ownership::Unmanaged);

  virtual ~Group();

  // Disable copy
  Group(const Group&) = delete;
  Group& operator=(const Group&) = delete;

  // Allow move
  Group(Group&& other) noexcept;
  Group& operator=(Group&& other) noexcept;

  // --- Object Management ---

  /**
   * @brief Add an object to the group.
   * @param obj The object to add.
   */
  void add_obj(Object& obj);
  void add_obj(lv_obj_t* obj);

  /**
   * @brief Remove an object from the group.
   * @param obj The object to remove.
   */
  void remove_obj(Object& obj);
  void remove_obj(lv_obj_t* obj);

  /**
   * @brief Remove all objects from the group.
   */
  void remove_all_objs();

  // --- Focus Management ---

  /**
   * @brief Focus on an object.
   * @param obj The object to focus.
   */
  void focus_obj(Object& obj);
  void focus_obj(lv_obj_t* obj);

  /**
   * @brief Focus the next object in the group.
   */
  void focus_next();

  /**
   * @brief Focus the previous object in the group.
   */
  void focus_prev();

  /**
   * @brief Freeze the group focus (prevent changing focus).
   * @param en true: freeze, false: unfreeze.
   */
  void focus_freeze(bool en);

  /**
   * @brief Set editing mode for the focused object.
   * @param edit true: edit mode, false: navigate mode.
   */
  void set_editing(bool edit);

  /**
   * @brief Set whether focus should wrap from last to first object.
   * @param en true: wrap enabled, false: wrap disabled.
   */
  void set_wrap(bool en);

  /**
   * @brief Get the currently focused object.
   * @return The raw object pointer, or nullptr if none.
   */
  lv_obj_t* get_focused() const;

  /**
   * @brief Get the raw lv_group_t pointer.
   * @return The raw pointer.
   */
  lv_group_t* raw() const;

  // --- Static Helpers ---

  /**
   * @brief Get the default group.
   * @return A Group wrapper around the default group (Unmanaged).
   */
  static Group get_default();

  /**
   * @brief Set the default group.
   * @param group The group to set as default.
   */
  static void set_default(Group& group);

  /**
   * @brief Swap the wrapped group with another.
   * @param other The other Group to swap with.
   */
  void swap(Group& other);

 private:
  lv_group_t* group_ = nullptr;
  Ownership ownership_ = Ownership::Unmanaged;
};

}  // namespace lvgl
